#include <gtk/gtk.h>
#include <epoxy/gl.h>

static float get_alpha(int i)
{
    switch (i)
    {
    case 0: return 0.00;
    case 1: return 0.25;
    case 2: return 0.50;
    case 3: return 0.75;
    case 4: return 1.00;
    }
    abort();
}

static gboolean render_callback(GtkGLArea *area, GdkGLContext *context, guint i)
{
    float alpha = get_alpha(i);
    gtk_gl_area_make_current(area);
    glClearColor(0, 0, 0, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
    return TRUE;
}
static gboolean render_callback0(GtkGLArea *area, GdkGLContext *context) { return render_callback(area, context, 0); }
static gboolean render_callback1(GtkGLArea *area, GdkGLContext *context) { return render_callback(area, context, 1); }
static gboolean render_callback2(GtkGLArea *area, GdkGLContext *context) { return render_callback(area, context, 2); }
static gboolean render_callback3(GtkGLArea *area, GdkGLContext *context) { return render_callback(area, context, 3); }
static gboolean render_callback4(GtkGLArea *area, GdkGLContext *context) { return render_callback(area, context, 4); }
typedef gboolean (*render_callback_t)(GtkGLArea *area, GdkGLContext *context);
static render_callback_t render_callbacks[] = {
    &render_callback0,
    &render_callback1,
    &render_callback2,
    &render_callback3,
    &render_callback4,
};

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GLArea Alpha Test");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, -1);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *desc = gtk_label_new("If working, you'll see 5 sections below with text at various alpha values.\n"
                                    "If broken, you'll see 5 black boxes with no text.");
    gtk_label_set_justify(GTK_LABEL(desc), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(box), desc, FALSE, FALSE, 0);

    for (guint i = 0; i < 5; i++)
    {
        float alpha = get_alpha(i);

        GtkWidget *label = gtk_label_new(NULL);
        {
            char buf[100];
            sprintf(buf, "Alpha %.2f", alpha);
            gtk_label_set_text(GTK_LABEL(label), buf);
        }

        GtkWidget *gl_area = gtk_gl_area_new();
        gtk_gl_area_set_has_alpha(GTK_GL_AREA(gl_area), TRUE);

        GtkWidget *overlay = gtk_overlay_new();
        gtk_container_add(GTK_CONTAINER(overlay), label);
        gtk_overlay_add_overlay(GTK_OVERLAY(overlay), gl_area);
        g_signal_connect(gl_area, "render", G_CALLBACK(render_callbacks[i]), NULL);
        gtk_box_pack_start(GTK_BOX(box), overlay, FALSE, FALSE, 0);
    }

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
