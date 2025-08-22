from camera import Camera
from sphere import Sphere

import taichi as ti
import taichi.math as tm

if __name__ == "__main__":
    ti.init(arch=ti.gpu)

    IMG_WIDTH = 1024
    IMG_HEIGHT = 1024

    sphere = Sphere(center=ti.Vector([0, 0, 0]), radius=1)

    camera = Camera(w=IMG_WIDTH, h=IMG_HEIGHT, fov=tm.pi)
    camera.update(camera_pos=ti.Vector([0, 0, -5]), target_pos=ti.Vector([0, 0, 0]))

    gui = ti.GUI("Ray Tracing", res=(IMG_WIDTH, IMG_HEIGHT))
    canvas = ti.Vector.field(3, dtype=ti.f32, shape=(IMG_WIDTH, IMG_HEIGHT))
    canvas.fill(0)

    @ti.kernel
    def render():
        for u, v in canvas:
            ray = camera.get_ray_by_pixel_uv(pixel_u=u, pixel_v=v)

            (is_hit, _, _, hit_point_normal) = sphere.intersect(ray=ray)

            alpha = 0.5 * (ray.dir[1] + 1.0)

            top_color = ti.Vector([0.5, 0.7, 1.0])
            bot_color = ti.Vector([1.0, 1.0, 1.0])

            color = (
                (hit_point_normal * 0.5 + 0.5)
                if is_hit
                else ((1.0 - alpha) * bot_color + alpha * top_color)
            )

            # Flip the vertical image.
            canvas[u, IMG_HEIGHT - v - 1] = color

    while gui.running:
        render()
        gui.set_image(canvas)
        gui.show()
