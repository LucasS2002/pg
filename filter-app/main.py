import dearpygui.dearpygui as dpg
import cv2 as cv
import cv2.typing as cvt
import numpy as np
import numpy.typing as npt
from sys import argv

import filter
import sticker

is_cam = None
active_filter = 0
active_sticker = 0
sticker_bg = None

def convert_image(image: cvt.MatLike) -> npt.NDArray:
    return np.asarray(cv.cvtColor(image, cv.COLOR_BGRA2RGBA), dtype=np.float32) / 255

def convert_frame(frame: cvt.MatLike) -> npt.NDArray:
    return np.asarray(np.flip(frame, 2), dtype=np.float32) / 255

def filter_callback() -> None:
    global active_filter
    active_filter = dpg.get_item_configuration("filters")["items"].index(dpg.get_value("filters"))

def sticker_selection_callback() -> None:
    global active_sticker
    active_sticker = dpg.get_item_configuration("stickers")["items"].index(dpg.get_value("stickers"))

def sticker_callback(sender: any, app_data: any, user_data: int | str) -> None:
    x, y = map(int, dpg.get_mouse_pos())

    global active_sticker, sticker_bg
    sticker_bg = sticker.apply(sticker_bg, sticker.list[active_sticker], x, y)
    dpg.set_value(user_data, convert_image(sticker_bg))

def save_callback(sender: any, app_data: any, user_data: cvt.MatLike) -> None:
    copy = user_data.copy()
    if copy.shape[-1] == 3:
        copy = cv.cvtColor(user_data, cv.COLOR_BGR2BGRA)

    global sticker_bg
    cv.imwrite("./output/image.png", sticker.apply(copy, sticker_bg))

def main() -> None:
    if len(argv) > 2:
        exit()

    global is_cam
    if (len(argv) == 1):
        is_cam = True

        cam = cv.VideoCapture(0)
        _, frame = cam.read()
        height, width, _ = frame.shape
    else:
        is_cam = False

        img = filtered_img = cv.imread(argv[1], cv.IMREAD_UNCHANGED)
        height, width, _ = img.shape

    global sticker_bg
    sticker_bg = np.zeros((height, width, 4), dtype=np.uint8)

    dpg.create_context()
    dpg.create_viewport(title=" ", width=width, height=height)
    dpg.setup_dearpygui()

    with dpg.texture_registry():
        if is_cam:
            cam_tag = dpg.add_raw_texture(width=width, height=height, default_value=convert_frame(frame), format=dpg.mvFormat_Float_rgb)
        else:
            img_tag = dpg.add_raw_texture(width=width, height=height, default_value=convert_image(img), format=dpg.mvFormat_Float_rgba)
        
        sticker_bg_tag = dpg.add_raw_texture(width=width, height=height, default_value=convert_image(sticker_bg), format=dpg.mvFormat_Float_rgba)
    
    with dpg.window(width=width, height=height) as main_window:
        if is_cam:
            dpg.add_image(texture_tag=cam_tag, pos=(0, 0))
        else:
            dpg.add_image(texture_tag=img_tag, pos=(0, 0))

        dpg.add_image(texture_tag=sticker_bg_tag, pos=(0, 0), tag="sticker_bg")

        dpg.set_primary_window(main_window, True)

    with dpg.window(label="Options"):
        dpg.add_text("Filters")
        dpg.add_radio_button(tag="filters", items=filter.names, callback=filter_callback)
        dpg.add_text("Stickers")
        dpg.add_radio_button(tag="stickers", items=sticker.names, callback=sticker_selection_callback)
        dpg.add_button(tag="save", label="Save", callback=save_callback)

    with dpg.item_handler_registry(tag="sticker handler"):
        dpg.add_item_clicked_handler(callback=sticker_callback, user_data=sticker_bg_tag)
        dpg.bind_item_handler_registry("sticker_bg", "sticker handler")

    dpg.show_viewport()
    while dpg.is_dearpygui_running():
        global active_filter

        if is_cam:
            _, frame = cam.read()
            filter.list[active_filter](frame)
            dpg.set_value(cam_tag, convert_frame(frame))
            dpg.set_item_user_data("save", frame)
        else:
            filtered_img = img.copy()
            filter.list[active_filter](filtered_img)
            dpg.set_item_user_data("save", filtered_img)
            dpg.set_value(img_tag, convert_image(filtered_img))

        dpg.render_dearpygui_frame()

    dpg.destroy_context()

if __name__ == "__main__":
    main()