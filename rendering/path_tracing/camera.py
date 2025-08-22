import taichi as ti
import taichi.math as tm
import numpy as np

from ray import Ray


@ti.data_oriented
class Camera:
    img_width: ti.types.f32
    img_height: ti.types.f32
    img_width_inv: ti.types.f32
    img_height_inv: ti.types.f32
    aspect_ratio: ti.types.f32
    fov: ti.types.f32

    viewport_width: ti.types.f32
    viewport_height: ti.types.f32
    viewport_width_inv: ti.types.f32
    viewport_height_inv: ti.types.f32

    pos: ti.types.vector(3, ti.f32)
    dir_left: ti.types.vector(3, ti.f32)
    dir_up: ti.types.vector(3, ti.f32)
    dir_forward: ti.types.vector(3, ti.f32)

    def __init__(
        self,
        w: float,
        h: float,
        fov: float,
    ):
        self.img_width = w
        self.img_height = h
        self.img_width_inv = 1 / w
        self.img_height_inv = 1 / h

        self.aspect_ratio = w / h
        self.fov = fov

        self.viewport_height = 2 * ti.tan(fov * 0.5)
        self.viewport_width = self.viewport_height * self.aspect_ratio
        self.viewport_height_inv = 1 / self.viewport_height
        self.viewport_width_inv = 1 / self.viewport_width

        self.pos = ti.Vector([0, 0, 0])
        self.dir_left = ti.Vector([1, 0, 0])
        self.dir_up = ti.Vector([0, 1, 0])
        self.dir_forward = ti.Vector([0, 0, 1])

    def update(self, camera_pos, target_pos):
        self.pos = camera_pos
        
        forward = target_pos.to_numpy() - camera_pos.to_numpy()
        forward = forward / np.linalg.norm(forward)

        left = np.linalg.cross(np.array([0, 1, 0]), forward)
        left = left / np.linalg.norm(left)

        up = np.linalg.cross(forward, left)
        up = up / np.linalg.norm(up)

        self.dir_forward = ti.Vector(forward.tolist())
        self.dir_left = ti.Vector(left.tolist())
        self.dir_up = ti.Vector(up.tolist())

    @ti.func
    def get_ray_by_pixel_uv(self, pixel_u: ti.types.f32, pixel_v: ti.types.f32):
        """
        Calculate camera rays from its center to the target pixel.
        """

        viewport_u = 1 - (2 * pixel_u + 1) * self.img_width_inv
        viewport_v = 1 - (2 * pixel_v + 1) * self.img_height_inv

        ray_dir = (
            self.dir_forward + viewport_u * self.dir_left + viewport_v * self.dir_up
        )

        return Ray(origin=self.pos, dir=tm.normalize(ray_dir))
