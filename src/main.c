#include "app.h"

int main(int argc, char **argv)
{
    GtkApplication *app = init_app();
    int status = run_app(app, argc, argv);
    clear_app(app);

    return status;
}