import taichi as ti
import taichi.math as tm

from ray import Ray


@ti.data_oriented
class Sphere:
    """
    Sphere class.
    """

    center: ti.types.vector(3, ti.f32)
    radius: ti.types.f32

    def __init__(self, center: ti.types.vector(3, ti.f32), radius: ti.types.f32):
        self.center = center
        self.radius = radius

    @ti.func
    def intersect(self, ray: Ray, t_min: float = 0.001, t_max: float = 1e8):
        """
        Used for get intersection from a ray to this sphere.
        """
        oc = self.center - ray.origin

        a = tm.dot(ray.dir, ray.dir)
        b = -2 * tm.dot(ray.dir, oc)
        c = tm.dot(oc, oc) - self.radius * self.radius

        discriminant = b * b - 4 * a * c

        is_hit = False
        hit_time = 0.0
        hit_point = ti.Vector([0.0, 0.0, 0.0])
        hit_point_normal = ti.Vector([0.0, 0.0, 0.0])

        if discriminant > 0:
            sqrtd = ti.sqrt(discriminant)
            hit_time = (-b - sqrtd) / (2 * a)

            if hit_time < t_min or hit_time > t_max:
                hit_time = (-b + sqrtd) / (2 * a)

            if t_min <= hit_time <= t_max:
                is_hit = True

        if is_hit:
            hit_point = ray.at(hit_time)
            hit_point_normal = tm.normalize(hit_point - self.center)

        return (is_hit, hit_time, hit_point, hit_point_normal)
