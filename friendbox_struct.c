#include <gtk/gtk.h>
#include "friendbox_struct.h"

#define MAX_WIDTH 20

/*******************************************************************************
 * utalk_friendbox struct
 * 
 * New widget: GtkWidget *utalk_friendbox_new (const gchar *avator_path, const gchar *name);
 * Get name  : const gchar *utalk_friendbox_get_name (GtkWidget *friendbox);
 * Set name  : void utalk_friendbox_set_name (GtkWidget *friendbox, const gchar *name);
 ******************************************************************************/

GtkWidget *utalk_friendbox_new (const gchar *avator_path, const gchar *name){
    // Get avator image
	GtkWidget *avator_image = gtk_image_new_from_file (avator_path);
	
	// Create friend name label
	GtkWidget *name_label = gtk_label_new (name);
	PangoAttrList *name_attr_list = pango_attr_list_new ();
	PangoAttribute *name_attr_scale = pango_attr_scale_new (1.2);
	pango_attr_list_insert (name_attr_list, name_attr_scale);
	gtk_label_set_attributes ((GtkLabel *)name_label, name_attr_list);
	pango_attr_list_unref (name_attr_list);
	
	// Combine with avator
	GtkWidget *friend_box_inside = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start (GTK_BOX(friend_box_inside), avator_image, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX(friend_box_inside), name_label, FALSE, TRUE, 0);

	GtkWidget *friendbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *up_separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
	GtkWidget *down_separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_box_pack_start (GTK_BOX(friendbox), up_separator, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX(friendbox), friend_box_inside, FALSE, TRUE, 0);
	gtk_box_pack_end (GTK_BOX(friendbox), down_separator, FALSE, TRUE, 0);

	return friendbox;
}

GtkWidget *utalk_friendbox_get_name_label (GtkWidget *friendbox){
	GList *list = gtk_container_get_children ((GtkContainer *)friendbox);
	list = list->next;
	list = gtk_container_get_children ((GtkContainer *)(list->data));
	list = list->next;
	return (GtkWidget *)(list->data);
}

const gchar *utalk_friendbox_get_name (GtkWidget *friendbox){
	GtkWidget *name_label = utalk_friendbox_get_name_label (friendbox);
	return gtk_label_get_text ((GtkLabel *)name_label);
}

void utalk_friendbox_set_name (GtkWidget *friendbox, const gchar *name){
	GtkWidget *name_label = utalk_friendbox_get_name_label (friendbox);
	gtk_label_set_text ((GtkLabel *)name_label, name);
}

/*******************************************************************************
 * utalk_friendbox_with_msg struct
 * 
 * New widget: GtkWidget *utalk_friendbox_with_msg_new (const gchar *avator_path, const gchar *name, const gchar *msg);
 * Get name  : const gchar *utalk_friendbox_with_msg_get_name (GtkWidget *friendbox);
 * Set name  : void utalk_friendbox_with_msg_set_name (GtkWidget *friendbox, const gchar *name);
 * Get msg   : const gchar *utalk_friendbox_with_msg_get_msg (GtkWidget *friendbox);
 * Set msg   : void utalk_friendbox_with_msg_set_msg (GtkWidget *friendbox, const gchar *name);
 ******************************************************************************/

GtkWidget *utalk_friendbox_with_msg_new (const gchar *avator_path, const gchar *name, const gchar *msg){
    // Get avator image
	GtkWidget *avator_image = gtk_image_new_from_file (avator_path);
	
	// Create friend name label
	GtkWidget *name_label = gtk_label_new (name);
	PangoAttrList *name_attr_list = pango_attr_list_new ();
	PangoAttribute *name_attr_scale = pango_attr_scale_new (1.2);
	pango_attr_list_insert (name_attr_list, name_attr_scale);
	gtk_label_set_attributes ((GtkLabel *)name_label, name_attr_list);
	pango_attr_list_unref (name_attr_list);
	
	// Create message label
	GtkWidget *msg_label = gtk_label_new (msg);
	
	// Combine name_label and msg_label
	GtkWidget *name_and_msg_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX(name_and_msg_box), TRUE);
	gtk_box_pack_start (GTK_BOX(name_and_msg_box), name_label, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX(name_and_msg_box), msg_label, FALSE, TRUE, 0);
	
	// Combine with avator
	GtkWidget *friend_box_inside = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start (GTK_BOX(friend_box_inside), avator_image, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX(friend_box_inside), name_and_msg_box, FALSE, TRUE, 0);

	GtkWidget *friend_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *up_separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
	GtkWidget *down_separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_box_pack_start (GTK_BOX(friend_box), up_separator, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX(friend_box), friend_box_inside, FALSE, TRUE, 0);
	gtk_box_pack_end (GTK_BOX(friend_box), down_separator, FALSE, TRUE, 0);

	return friend_box;
}

GtkWidget *utalk_friendbox_with_msg_get_name_label (GtkWidget *friendbox){
	GList *list = gtk_container_get_children ((GtkContainer *)friendbox);
	list = list->next;
	list = gtk_container_get_children ((GtkContainer *)(list->data));
	list = list->next;
	list = gtk_container_get_children ((GtkContainer *)(list->data));
	return (GtkWidget *)(list->data);
}

const gchar *utalk_friendbox_with_msg_get_name (GtkWidget *friendbox){
	GtkWidget *name_label = utalk_friendbox_with_msg_get_name_label (friendbox);
	return gtk_label_get_text ((GtkLabel *)name_label);
}

void utalk_friendbox_with_msg_set_name (GtkWidget *friendbox, const gchar *name){
	GtkWidget *name_label = utalk_friendbox_with_msg_get_name_label (friendbox);
	gtk_label_set_text ((GtkLabel *)name_label, name);
}

GtkWidget *utalk_friendbox_with_msg_get_msg_label (GtkWidget *friendbox){
	GList *list = gtk_container_get_children ((GtkContainer *)friendbox);
	list = list->next;
	list = gtk_container_get_children ((GtkContainer *)(list->data));
	list = list->next;
	list = gtk_container_get_children ((GtkContainer *)(list->data));
	list = list->next;
	return (GtkWidget *)(list->data);
}

const gchar *utalk_friendbox_with_msg_get_msg (GtkWidget *friendbox){
	GtkWidget *name_label = utalk_friendbox_with_msg_get_msg_label (friendbox);
	return gtk_label_get_text ((GtkLabel *)name_label);
}

void utalk_friendbox_with_msg_set_msg (GtkWidget *friendbox, const gchar *msg){
	GtkWidget *msg_label = utalk_friendbox_with_msg_get_msg_label (friendbox);
	gchar s[30];
	if (strlen (msg) > MAX_WIDTH){
		strncpy (s, msg, MAX_WIDTH - 3);
		s[MAX_WIDTH - 3] = '.';
		s[MAX_WIDTH - 2] = '.';
		s[MAX_WIDTH - 1] = '.';
		s[MAX_WIDTH] = '\0';
	}
	else{
		strcpy (s, msg);
	}
	gtk_label_set_text ((GtkLabel *)msg_label, s);
}