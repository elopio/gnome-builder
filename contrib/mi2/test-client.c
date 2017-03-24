/* test-client.c
 *
 * Copyright (C) 2017 Christian Hergert <chergert@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mi2-client.h"
#include "mi2-error.h"

static GMainLoop *main_loop;

static GIOStream *
create_io_stream_to_gdb (void)
{
  g_autoptr(GSubprocess) subprocess = NULL;
  g_autoptr(GIOStream) io_stream = NULL;
  g_autoptr(GError) error = NULL;

  subprocess = g_subprocess_new (G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDOUT_PIPE,
                                 &error,
                                 "gdb", "--interpreter", "mi2",
                                 NULL);
  g_assert_no_error (error);
  g_assert (subprocess);

  io_stream = g_simple_io_stream_new (g_subprocess_get_stdout_pipe (subprocess),
                                      g_subprocess_get_stdin_pipe (subprocess));

  g_subprocess_wait_async (subprocess, NULL, NULL, NULL);

  return g_steal_pointer (&io_stream);
}

static void
log_handler (Mi2Client   *client,
             const gchar *log)
{
  g_print ("%s", log);
}

static void
thread_group_added (Mi2Client       *client,
                    Mi2EventMessage *message,
                    gpointer         user_data)
{
}

static void
event (Mi2Client       *client,
       Mi2EventMessage *message,
       gpointer         user_data)
{
  g_print ("EVENT: %s\n", mi2_event_message_get_name (message));
}

static void
stack_info_frame_cb (GObject      *object,
                     GAsyncResult *result,
                     gpointer      user_data)
{
  Mi2Client *client = (Mi2Client *)object;
  g_autoptr(GError) error = NULL;
  gboolean r;

  g_assert (MI2_IS_CLIENT (client));
  g_assert (G_IS_ASYNC_RESULT (result));

  r = mi2_client_exec_finish (client, result, &error);
  g_assert_error (error, MI2_ERROR, MI2_ERROR_UNKNOWN_ERROR);
  g_assert_cmpstr (error->message, ==, "No registers.");
  g_assert_cmpint (r, ==, FALSE);

  g_main_loop_quit (main_loop);
}

gint
main (gint argc,
      gchar *argv[])
{
  g_autoptr(Mi2Client) client = NULL;
  g_autoptr(GIOStream) io_stream = NULL;

  main_loop = g_main_loop_new (NULL, FALSE);
  io_stream = create_io_stream_to_gdb ();
  client = mi2_client_new (io_stream);

  g_signal_connect (client, "log", G_CALLBACK (log_handler), NULL);
  g_signal_connect (client, "event::thread-group-added", G_CALLBACK (thread_group_added), NULL);
  g_signal_connect (client, "event", G_CALLBACK (event), NULL);

  mi2_client_start_listening (client);

  mi2_client_exec_async (client,
                         "stack-info-frame",
                         NULL,
                         stack_info_frame_cb,
                         NULL);

  g_main_loop_run (main_loop);
  g_main_loop_unref (main_loop);

  return 0;
}