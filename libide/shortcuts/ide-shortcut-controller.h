/* ide-shortcut-controller.h
 *
 * Copyright (C) 2016 Christian Hergert <chergert@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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

#ifndef IDE_SHORTCUT_CONTROLLER_H
#define IDE_SHORTCUT_CONTROLLER_H

#include <gtk/gtk.h>

#include "ide-shortcut-context.h"

G_BEGIN_DECLS

#define IDE_TYPE_SHORTCUT_CONTROLLER (ide_shortcut_controller_get_type())

G_DECLARE_FINAL_TYPE (IdeShortcutController, ide_shortcut_controller, IDE, SHORTCUT_CONTROLLER, GObject)

IdeShortcutController  *ide_shortcut_controller_new                (GtkWidget             *widget);
gboolean                ide_shortcut_controller_handle_event       (IdeShortcutController *self,
                                                                    const GdkEventKey     *event);
IdeShortcutController  *ide_shortcut_controller_find               (GtkWidget             *widget);
IdeShortcutContext     *ide_shortcut_controller_get_context        (IdeShortcutController *self);
void                    ide_shortcut_controller_set_context        (IdeShortcutController *self,
                                                                    IdeShortcutContext    *context);
const IdeShortcutChord *ide_shortcut_controller_get_current_chord  (IdeShortcutController *self);
void                    ide_shortcut_controller_add_command_signal (IdeShortcutController *self,
                                                                    const gchar           *command_id,
                                                                    const gchar           *default_accel,
                                                                    const gchar           *group,
                                                                    const gchar           *title,
                                                                    const gchar           *subtitle,
                                                                    const gchar           *signal_name,
                                                                    guint                  n_args,
                                                                    ...);

G_END_DECLS

#endif /* IDE_SHORTCUT_CONTROLLER_H */
