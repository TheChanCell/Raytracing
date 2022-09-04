#pragma once

#include <utility>

#include "hittable.h"
#include "texture.h"
#include "material.h"

class constant_medium : public hittable
{
public:
	constant_medium(shared_ptr<hittable> b, double d, shared_ptr<texture> a)
	: boundary(std::move(b)), phase_function(make_shared<isotropic>(a)), neg_inv_density(-1/d) {}

	constant_medium(shared_ptr<hittable> b, double d, color c)
	: boundary(std::move(b)), phase_function(make_shared<isotropic>(c)), neg_inv_density(-1/d) {}

	bool hit(const ray& r, hit_record& rec, double t_max, double t_min) const override;

	bool bounding_box(double time0, double time1, aabb& output_box) const override
	{
		return boundary->bounding_box(time0, time1, output_box);
	}

	shared_ptr<hittable> boundary;
	shared_ptr<material> phase_function;
	double neg_inv_density;
};

inline bool constant_medium::hit(const ray& r, hit_record& rec, double t_max, double t_min) const
{

    const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.00001;

    hit_record rec1, rec2;

    if (!boundary->hit(r, rec1, Infinity, -Infinity))
        return false;

    if (!boundary->hit(r, rec2, Infinity, rec1.t + 0.0001))
        return false;

    if (debugging) std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto ray_length = r.get_dir().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.pos = r.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " << hit_distance << '\n'
            << "rec.t = " << rec.t << '\n'
            << "rec.p = " << rec.pos << '\n';
    }

    rec.normal = vec3(1, 0, 0);  // arbitrary
    rec.front_face = true;     // also arbitrary
    rec.mat_ptr = phase_function;

    return true;
}

