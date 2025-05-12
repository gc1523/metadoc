#include <gtk/gtk.h>
#include "../include/metadoc.h"

static void on_encrypt_clicked(GtkButton *button, gpointer user_data) {
    GtkFileChooser *key_chooser = GTK_FILE_CHOOSER(g_object_get_data(G_OBJECT(button), "key_chooser"));
    GtkFileChooser *target_chooser = GTK_FILE_CHOOSER(g_object_get_data(G_OBJECT(button), "target_chooser"));

    char *file1 = gtk_file_chooser_get_filename(key_chooser);
    char *file2 = gtk_file_chooser_get_filename(target_chooser);

    if (file1 && file2) {
        g_print("Encrypting...\n");
        g_print("Key File: %s\n", file1);
        g_print("Target File: %s\n", file2);
        encrypt(file1, file2);
    } else {
        g_print("Please select both files.\n");
    }

    g_free(file1);
    g_free(file2);
}

static void on_decrypt_clicked(GtkButton *button, gpointer user_data) {
    GtkFileChooser *key_chooser = GTK_FILE_CHOOSER(g_object_get_data(G_OBJECT(button), "key_chooser"));
    GtkFileChooser *target_chooser = GTK_FILE_CHOOSER(g_object_get_data(G_OBJECT(button), "target_chooser"));

    char *file1 = gtk_file_chooser_get_filename(key_chooser);
    char *file2 = gtk_file_chooser_get_filename(target_chooser);

    if (file1 && file2) {
        g_print("Decrypting...\n");
        g_print("Key File: %s\n", file1);
        g_print("Target File: %s\n", file2);
        // NOTE: this is currently dangerous asf, as we arent copying the file
        // to a temp file, and then decrypting it, we are overwriting the original
        // file, which is not good. probably best to write to a temp file, and then
        // rename it to the original file name after decryption. Then delete the original. 😈👿
        decrypt(file1, file2);
    } else {
        g_print("Please select both files.\n");
    }

    g_free(file1);
    g_free(file2);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Window setup
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "metadoc");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Colours (Wow)
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        "window { background-color:rgb(127, 155, 212); }",
        -1, NULL);

    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    // Layout container
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Label for File chooser 1
    GtkWidget *key_label = gtk_label_new("Choose the file to encrypt/decrypt:");
    gtk_box_pack_start(GTK_BOX(box), key_label, FALSE, FALSE, 0);

    // File chooser 1
    GtkWidget *key_chooser = gtk_file_chooser_button_new("Select file to encrypt/decrypt", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_box_pack_start(GTK_BOX(box), key_chooser, FALSE, FALSE, 0);

    // Label for File chooser 2
    GtkWidget *target_label = gtk_label_new("Choose the key file:");
    gtk_box_pack_start(GTK_BOX(box), target_label, FALSE, FALSE, 0);

    // File chooser 2
    GtkWidget *target_chooser = gtk_file_chooser_button_new("Select key file", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_box_pack_start(GTK_BOX(box), target_chooser, FALSE, FALSE, 0);

    // Horizontal box for buttons
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_end(GTK_BOX(box), button_box, FALSE, FALSE, 0);

    // Encrypt Button
    GtkWidget *encrypt_button = gtk_button_new_with_label("Encrypt");
    gtk_widget_set_hexpand(encrypt_button, TRUE); // Make the button expand horizontally
    gtk_box_pack_start(GTK_BOX(button_box), encrypt_button, TRUE, TRUE, 0);

    // Decrypt Button
    GtkWidget *decrypt_button = gtk_button_new_with_label("Decrypt");
    gtk_widget_set_hexpand(decrypt_button, TRUE); // Make the button expand horizontally
    gtk_box_pack_start(GTK_BOX(button_box), decrypt_button, TRUE, TRUE, 0);

    // file chooser callback shii
    g_object_set_data(G_OBJECT(encrypt_button), "key_chooser", key_chooser);
    g_object_set_data(G_OBJECT(encrypt_button), "target_chooser", target_chooser);
    g_signal_connect(encrypt_button, "clicked", G_CALLBACK(on_encrypt_clicked), NULL);

    g_object_set_data(G_OBJECT(decrypt_button), "key_chooser", key_chooser);
    g_object_set_data(G_OBJECT(decrypt_button), "target_chooser", target_chooser);
    g_signal_connect(decrypt_button, "clicked", G_CALLBACK(on_decrypt_clicked), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
