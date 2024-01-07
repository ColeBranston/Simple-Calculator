// Include necessary libraries
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define a structure to hold the calculator's widgets
typedef struct
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button[17];
} calc;

GtkWidget *box;

#define SIZE 10

// Initialize buffers and flags
char input_buffer[100] = {0};
char output_buffer[100] = {0};

bool clear_buffer = false;
bool add = false;
bool mul = false;
bool divv = false;
bool sub = false;

// Initialize result and number array
float result = 0.0;
static float num[SIZE];
int count = 0;

// Function to perform calculations
static void calculate(GtkButton *button, gpointer data)
{
    // Get the label of the button that was clicked
    const gchar *text = gtk_button_get_label(button);

    // Check if the button was an operator or equals sign
    if ((strcmp("+", text) == 0) || (strcmp("-", text) == 0) || (strcmp("/", text) == 0) || (strcmp("x", text) == 0) || (strcmp("=", text) == 0))
    {
        // Convert the input buffer to a float and store it in the number array
        num[count] = atof(input_buffer);
        count++;
        clear_buffer = true;

        // Set the appropriate operation flag
        if (strcmp("+", text) == 0)
        {
            add = true;
        }
        if (strcmp("-", text) == 0)
        {
            sub = true;
        }
        if (strcmp("/", text) == 0)
        {
            divv = true;
        }
        if (strcmp("x", text) == 0)
        {
            mul = true;
        }
    }

    // If the equals sign was clicked, perform the calculation
    if (strcmp("=", text) == 0)
    {
        int x = sizeof(num) / sizeof(num[0]);

        if (add)
        {
            for (int i = 0; i < x; i++)
            {
                result += num[i];
            }
        }

        if (divv)
        {
            result = num[0] / num[1];
        }

        if (sub)
        {
            if (result == 0.0)
            {
                result = num[0] * 2;
            }
            for (int i = 0; i < x; i++)
            {
                result -= num[i];
            }
        }

        if (mul)
        {
            result = num[0] * num[1];
        }

        // Reset the operation flags
        add = false;
        mul = false;
        divv = false;
        sub = false;

        // Update the display with the result
        gtk_entry_set_text(GTK_ENTRY(box), "");
        sprintf(output_buffer, "%.3f", result);
        gtk_entry_set_text(GTK_ENTRY(box), output_buffer);
        result = 0.0;
    }
    else
    {
        // If the buffer needs to be cleared, clear it
        if (clear_buffer)
        {
            memset(input_buffer, 0, strlen(input_buffer));
            clear_buffer = false;
        }
        else
        {
            // Otherwise, append the button label to the input buffer
            strncat(input_buffer, text, 1);
        }

        // Append the button label to the output buffer and update the display
        strncat(output_buffer, text, 1);
        gtk_entry_set_text(GTK_ENTRY(box), output_buffer);
    }

    // If the clear button was clicked, reset everything
    if (strcmp("C", text) == 0)
    {
        gtk_entry_set_text(GTK_ENTRY(box), "");
        memset(input_buffer, 0, strlen(input_buffer));
        memset(output_buffer, 0, strlen(output_buffer));

        count = 0;
        int x = sizeof(num) / sizeof(num[0]);

        for (int i = 0; i < x; i++)
        {
            num[i] = 0;
        }

        add = false;
        mul = false;
        divv = false;
        sub = false;
    }
}

// Function to initialize the application
static void activate(GtkApplication *app, gpointer user_data)
{
    calc widget;

    // Create a new window
    widget.window = gtk_application_window_new(app);
    gtk_window_set_position(GTK_WINDOW(widget.window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(widget.window), "My Calculator");
    gtk_window_set_default_size(GTK_WINDOW(widget.window), 200, 200);
    gtk_container_set_border_width(GTK_CONTAINER(widget.window), 10);

    // Create a new grid and add it to the window
    widget.grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(widget.window), widget.grid);

    // Create a new entry box and make it uneditable
    box = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(box), FALSE);

    // Create the calculator buttons
    widget.button[0] = gtk_button_new_with_label("0");
    widget.button[1] = gtk_button_new_with_label("1");
    widget.button[2] = gtk_button_new_with_label("2");
    widget.button[3] = gtk_button_new_with_label("3");
    widget.button[4] = gtk_button_new_with_label("4");
    widget.button[5] = gtk_button_new_with_label("5");
    widget.button[6] = gtk_button_new_with_label("6");
    widget.button[7] = gtk_button_new_with_label("7");
    widget.button[8] = gtk_button_new_with_label("8");
    widget.button[9] = gtk_button_new_with_label("9");
    widget.button[10] = gtk_button_new_with_label("+");
    widget.button[11] = gtk_button_new_with_label("-");
    widget.button[12] = gtk_button_new_with_label("/");
    widget.button[13] = gtk_button_new_with_label("x");
    widget.button[14] = gtk_button_new_with_label("=");
    widget.button[15] = gtk_button_new_with_label(".");
    widget.button[16] = gtk_button_new_with_label("C");

    // Connect the "clicked" signal of each button to the calculate function
    for (int i = 0; i < 17; i++)
    {
        g_signal_connect(widget.button[i], "clicked", G_CALLBACK(calculate), NULL);
    }

    // Add the buttons to the grid
    for (int i = 0; i < 17; i++)
    {
        gtk_grid_attach(GTK_GRID(widget.grid), widget.button[i], i % 4, i / 4, 1, 1);
    }

    // Add the entry box to the grid
    gtk_grid_attach(GTK_GRID(widget.grid), box, 0, 4, 4, 1);

    // Show all widgets
    gtk_widget_show_all(widget.window);
}

// Main function
int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    // Create a new GtkApplication
    app = gtk_application_new("com.github.lascreations.calculator", G_APPLICATION_FLAGS_NONE);

    // Connect the "activate" signal to the activate function
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Clean up
    g_object_unref(app);

    return status;
}
}
