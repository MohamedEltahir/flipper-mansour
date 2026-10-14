
#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>

static void draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 25, 15, "Mansour,");

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 38, 34, "fuck you");
    canvas_draw_str(canvas, 34, 49, "very much");
}

static void input_callback(InputEvent* event, void* context) {
    FuriMessageQueue* queue = context;
    furi_message_queue_put(queue, event, 0);
}

int32_t mansour_app(void* context) {
    UNUSED(context);

    FuriMessageQueue* queue =
        furi_message_queue_alloc(8, sizeof(InputEvent));

    ViewPort* viewport = view_port_alloc();

    view_port_draw_callback_set(
        viewport, draw_callback, NULL);

    view_port_input_callback_set(
        viewport, input_callback, queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, viewport, GuiLayerFullscreen);

    InputEvent event;

    while(true) {
        if(furi_message_queue_get(
               queue, &event, FuriWaitForever) == FuriStatusOk) {
            if(event.key == InputKeyBack &&
               event.type == InputTypeShort) {
                break;
            }
        }
    }

    gui_remove_view_port(gui, viewport);
    view_port_free(viewport);
    furi_record_close(RECORD_GUI);
    furi_message_queue_free(queue);

    return 0;
}
