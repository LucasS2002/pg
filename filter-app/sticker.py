import cv2 as cv
import cv2.typing as cvt

def apply(background: cvt.MatLike, sticker: cvt.MatLike, pos_x: int | None = None, pos_y: int | None = None) -> cvt.MatLike:
    b, g, r, a = cv.split(sticker)

    f_rows, f_cols, _ = sticker.shape
    b_rows, b_cols, _ = background.shape

    if pos_x is None:
        pos_x = b_cols // 2
    if pos_y is None:
        pos_y = b_rows // 2

    x_start = pos_x - f_cols // 2
    y_start = pos_y - f_rows // 2

    bg_x_start = max(0, x_start)
    bg_y_start = max(0, y_start)
    bg_x_end = min(b_cols, x_start + f_cols)
    bg_y_end = min(b_rows, y_start + f_rows)

    fg_x_start = max(0, -x_start)
    fg_y_start = max(0, -y_start)
    fg_x_end = fg_x_start + (bg_x_end - bg_x_start)
    fg_y_end = fg_y_start + (bg_y_end - bg_y_start)

    sticker = sticker[fg_y_start:fg_y_end, fg_x_start:fg_x_end]
    mask = a[fg_y_start:fg_y_end, fg_x_start:fg_x_end]
    mask_inv = cv.bitwise_not(mask)
    roi = background[bg_y_start:bg_y_end, bg_x_start:bg_x_end]

    img_bg = cv.bitwise_and(roi, roi, mask=mask_inv)
    img_fg = cv.bitwise_and(sticker, sticker, mask=mask)
    res = cv.add(img_bg, img_fg)

    background[bg_y_start:bg_y_end, bg_x_start:bg_x_end] = res

    return background

names = ["Anger", "Exclamation", "Explosion", "Eyeglasses", "Star"]
list = [
    cv.imread("./assets/anger.png", cv.IMREAD_UNCHANGED),
    cv.imread("./assets/exclamation.png", cv.IMREAD_UNCHANGED),
    cv.imread("./assets/explosion.png", cv.IMREAD_UNCHANGED),
    cv.imread("./assets/eyeglasses.png", cv.IMREAD_UNCHANGED),
    cv.imread("./assets/star.png", cv.IMREAD_UNCHANGED),
]