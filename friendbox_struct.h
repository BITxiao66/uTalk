#ifndef __FRIENDBOX_STRUCT_H
#define __FRIENDBOX_STRUCT_H

#include <string.h>

extern GtkWidget *utalk_friendbox_new (const gchar *avator_path, const gchar *name);
extern const gchar *utalk_friendbox_get_name (GtkWidget *friendbox);
extern void utalk_friendbox_set_name (GtkWidget *friendbox, const gchar *name);

extern GtkWidget *utalk_friendbox_with_msg_new (const gchar *avator_path, const gchar *name, const gchar *msg);
extern const gchar *utalk_friendbox_with_msg_get_name (GtkWidget *friendbox);
extern void utalk_friendbox_with_msg_set_name (GtkWidget *friendbox, const gchar *name);
extern const gchar *utalk_friendbox_with_msg_get_msg (GtkWidget *friendbox);
extern void utalk_friendbox_with_msg_set_msg (GtkWidget *friendbox, const gchar *msg);

#endif