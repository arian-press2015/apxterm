#include "app.h"
#include "signals.h"

int main(int argc, char **argv)
{
    GtkApplication *app = init_app();
    connect_app_signals(app);
    int status = run_app(app, argc, argv);
    clear_app(app);

    return status;
}