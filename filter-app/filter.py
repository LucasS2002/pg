import cv2 as cv
import cv2.typing as cvt
import numpy as np

def dummy(_: any) -> None:
    return

def bitwise_not(image: cvt.MatLike) -> None:
    cv.bitwise_not(src=image, dst=image)

def black_white(image):
    if image.shape[-1] == 4:
        cv.cvtColor(src=cv.cvtColor(image, cv.COLOR_BGRA2GRAY), code=cv.COLOR_GRAY2BGRA, dst=image)
    elif image.shape[-1] == 3:
        cv.cvtColor(src=cv.cvtColor(image, cv.COLOR_BGR2GRAY), code=cv.COLOR_GRAY2BGR, dst=image)

def blur(image: cvt.MatLike) -> None:
    cv.blur(src=image, ksize=(5, 5), dst=image)

def box(image: cvt.MatLike) -> None:
    cv.boxFilter(src=image, ddepth=-1, ksize=(50, 50), dst=image)

def gaussian_blur(image: cvt.MatLike) -> None:
    cv.GaussianBlur(src=image, ksize=(5, 5), dst=image, sigmaX=0, sigmaY=0)

def darken(image: cvt.MatLike) -> None:
    cv.convertScaleAbs(src=image, beta=-50, dst=image)

def emboss(image: cvt.MatLike) -> None:
    kernel = np.array([[0, -3, -3], [3, 0, -3], [3, 3, 0]])

    if image.shape[-1] == 4:
        cv.cvtColor(src=cv.filter2D(src=cv.cvtColor(src=image, code=cv.COLOR_BGRA2GRAY), ddepth=-1, kernel=kernel), code=cv.COLOR_GRAY2BGRA, dst=image)
    elif image.shape[-1] == 3:
        cv.cvtColor(src=cv.filter2D(src=cv.cvtColor(src=image, code=cv.COLOR_BGR2GRAY), ddepth=-1, kernel=kernel), code=cv.COLOR_GRAY2BGR, dst=image)

def lighten(image: cvt.MatLike) -> None:
    cv.convertScaleAbs(src=image, beta=50, dst=image)

def outline(image: cvt.MatLike) -> None:
    kernel = np.array([[-1, -1, -1], [-1, 9, -1], [-1, -1, -1]])
    cv.filter2D(src=image, ddepth=-1, kernel=kernel, dst=image)

def sharpen(image: cvt.MatLike) -> None:
    kernel = np.array([[0, -1, 0], [-1, 5, -1], [0, -1, 0]])
    cv.filter2D(src=image, ddepth=-1, kernel=kernel, dst=image)

def sobel(image: cvt.MatLike) -> None:
    kernel = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])

    if image.shape[-1] == 4:
        cv.cvtColor(src=cv.filter2D(src=cv.cvtColor(src=image, code=cv.COLOR_BGRA2GRAY), ddepth=-1, kernel=kernel), code=cv.COLOR_GRAY2BGRA, dst=image)
    elif image.shape[-1] == 3:
        cv.cvtColor(src=cv.filter2D(src=cv.cvtColor(src=image, code=cv.COLOR_BGR2GRAY), ddepth=-1, kernel=kernel), code=cv.COLOR_GRAY2BGR, dst=image)

names = ["None", "Bitwise Not", "Black & White", "Blur", "Box", "Darken", "Emboss", "Gaussian Blur", "Lighten", "Outline", "Sharpen", "Sobel"]
list = [dummy, bitwise_not, black_white, blur, box, darken, emboss, gaussian_blur, lighten, outline, sharpen, sobel]