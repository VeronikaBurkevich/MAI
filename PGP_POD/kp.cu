#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

typedef unsigned char uchar;
typedef unsigned uint;
typedef float flt;

using namespace std;

#define FLT_1_PHI      (0.61803398874989485F)
#define FLT_PHI        (1.61803398874989484F)
#define FLT_PI         (3.14159265358979323F)
#define FLT_SQRT3      (1.73205080756887729F)
#define FLT_SQRT_PHI_2 (1.90211303259030714F)

#define exit_if(cnd_value, msg) \
    do { \
        if ((cnd_value)) { \
            if (errno) \
                perror((msg)); \
            else \
                fprintf(stderr, "error: %s\n", (msg)); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

#define cudaErrorCheck(error) \
    do { \
        cudaError_t result = (error); \
        if (res != cudaSuccess) { \
            fprintf(stderr, "cuda %s:%d error: %s\n", __FILE__, __LINE__, \
                cudaGetErrorString(res)); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)

struct triangle {
    float3 a, b, c;
    uchar4 color;
};

static const uchar4
    // aqua         = {   0, 255, 255, 0 },
    darkgreen    = {   0, 100,   0, 0 },
    darkred      = { 139,   0,   0, 0 },
    gold         = { 255, 215,   0, 0 },
    green        = {   0, 255,   0, 0 },
    // khaki        = { 240, 230, 140, 0 },
    // magenta      = { 255,   0, 255, 0 },
    maroon       = { 128,   0,   0, 0 },
    midnightblue = {  25,  25, 112, 0 },
    purple       = { 128,   0, 128, 0 },
    red          = { 255,   0,   0, 0 },
    teal         = {   0, 128, 128, 0 },
    yellow       = { 255, 255,   0, 0 };

__device__ __host__ static float3 cross(const float3 a, const float3 b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

__device__ __host__ static flt dot(const float3 a, const float3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

__device__ __host__ static float3 float3_multiplies(
    const float3 a,
    const flt alpha
) {
    return { a.x * alpha, a.y * alpha, a.z * alpha };
}

__device__ __host__ static float3 float3_plus(
    const float3 a,
    const float3 b
) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

__device__ __host__ static float3 float3_fma(
    const float3 a,
    const flt radius,
    const float3 center
) {
    return float3_plus(float3_multiplies(a, radius), center);
}

__device__ __host__ static flt length(const float3 a) {
    return sqrtf(dot(a, a));
}

__device__ __host__ static float3 float3_minus(const float3 a, const float3 b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

__device__ __host__ static float3 float3_multiplies(
    const float3 a,
    const float3 b
) {
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}

__device__ __host__ static float3 normalize(const float3 a) {
    const flt a_length = length(a);
    return { a.x / a_length, a.y / a_length, a.z / a_length };
}

__device__ __host__ float3 mult(
    const float3 a,
    const float3 b,
    const float3 c,
    const float3 all
) {
    return {
        a.x * all.x + b.x * all.y + c.x * all.z,
        a.y * all.x + b.y * all.y + c.y * all.z,
        a.z * all.x + b.z * all.y + c.z * all.z
    };
}

static void make_dodecahedron(
    vector<triangle> &triangles,
    const flt radius,
    const float3 center
) {
    static const float3
        point_1  = { -FLT_1_PHI / FLT_SQRT3, 0.0F, FLT_PHI / FLT_SQRT3 },
        point_2  = {  FLT_1_PHI / FLT_SQRT3, 0.0F, FLT_PHI / FLT_SQRT3 },
        point_3  = { -1.0F / FLT_SQRT3, 1.0F / FLT_SQRT3, 1.0F / FLT_SQRT3 },
        point_4  = { 1.0F / FLT_SQRT3, 1.0F / FLT_SQRT3, 1.0F / FLT_SQRT3 },
        point_5  = { 1.0F / FLT_SQRT3, -1.0F / FLT_SQRT3, 1.0F / FLT_SQRT3 },
        point_6  = { -1.0F / FLT_SQRT3, -1.0F / FLT_SQRT3, 1.0F / FLT_SQRT3 },
        point_7  = { 0.0F, -FLT_PHI / FLT_SQRT3, FLT_1_PHI / FLT_SQRT3 },
        point_8  = { 0.0F, FLT_PHI / FLT_SQRT3, FLT_1_PHI / FLT_SQRT3 },
        point_9  = { -FLT_PHI / FLT_SQRT3, -FLT_1_PHI / FLT_SQRT3, 0.0F },
        point_10 = { -FLT_PHI / FLT_SQRT3, FLT_1_PHI / FLT_SQRT3, 0.0F },
        point_11 = { FLT_PHI / FLT_SQRT3, FLT_1_PHI / FLT_SQRT3, 0.0F },
        point_12 = { FLT_PHI / FLT_SQRT3, -FLT_1_PHI / FLT_SQRT3, 0.0F },
        point_13 = { 0.0F, -FLT_PHI / FLT_SQRT3, -FLT_1_PHI / FLT_SQRT3 },
        point_14 = { 0.0F, FLT_PHI / FLT_SQRT3, -FLT_1_PHI / FLT_SQRT3 },
        point_15 = { 1.0F / FLT_SQRT3, 1.0F / FLT_SQRT3, -1.0F / FLT_SQRT3 },
        point_16 = { 1.0F / FLT_SQRT3, -1.0F / FLT_SQRT3, -1.0F / FLT_SQRT3 },
        point_17 = { -1.0F / FLT_SQRT3, -1.0F / FLT_SQRT3, -1.0F / FLT_SQRT3 },
        point_18 = { -1.0F / FLT_SQRT3, 1.0F / FLT_SQRT3, -1.0F / FLT_SQRT3 },
        point_19 = {  FLT_1_PHI / FLT_SQRT3, 0.0F, -FLT_PHI / FLT_SQRT3 },
        point_20 = { -FLT_1_PHI / FLT_SQRT3 , 0.0F, -FLT_PHI / FLT_SQRT3 };

    triangles.push_back({ point_5, point_1, point_7, green });
    triangles.push_back({ point_1, point_6, point_7, darkgreen });
    triangles.push_back({ point_1, point_5, point_2, maroon });
    triangles.push_back({ point_1, point_4, point_8, purple });
    triangles.push_back({ point_3, point_1, point_8, red });
    triangles.push_back({ point_1, point_2, point_4, yellow });
    triangles.push_back({ point_11, point_2, point_12, green });
    triangles.push_back({ point_4, point_2, point_11, darkgreen });
    triangles.push_back({ point_2, point_5, point_12, maroon });
    triangles.push_back({ point_6, point_1, point_9, purple });
    triangles.push_back({ point_1, point_3, point_10, red });
    triangles.push_back({ point_9, point_1, point_10, yellow });
    triangles.push_back({ point_6, point_9, point_17, green });
    triangles.push_back({ point_7, point_6, point_13, darkgreen });
    triangles.push_back({ point_13, point_6, point_17, maroon });
    triangles.push_back({ point_5, point_13, point_16, purple });
    triangles.push_back({ point_5, point_7, point_13, red });
    triangles.push_back({ point_12, point_5, point_16, green });
    triangles.push_back({ point_3, point_14, point_18, darkgreen });
    triangles.push_back({ point_3, point_8, point_14, maroon });
    triangles.push_back({ point_10, point_3, point_18, purple });
    triangles.push_back({ point_14, point_4, point_15, red });
    triangles.push_back({ point_8, point_4, point_14, yellow });
    triangles.push_back({ point_4, point_11, point_15, green });
    triangles.push_back({ point_9, point_18, point_20, darkgreen });
    triangles.push_back({ point_17, point_9, point_20, maroon });
    triangles.push_back({ point_9, point_10, point_18, purple });
    triangles.push_back({ point_15, point_12, point_19, red });
    triangles.push_back({ point_12, point_16, point_19, green });
    triangles.push_back({ point_11, point_12, point_15, darkgreen });
    triangles.push_back({ point_13, point_20, point_19, maroon });
    triangles.push_back({ point_16, point_13, point_19, purple });
    triangles.push_back({ point_13, point_17, point_20, yellow });
    triangles.push_back({ point_20, point_14, point_19, maroon });
    triangles.push_back({ point_18, point_14, point_20, darkgreen });
    triangles.push_back({ point_14, point_15, point_19, yellow });

    for (size_t i = 0; i < 36U; ++i) {
        triangle &triangle_ref = triangles[triangles.size() - i - 1U];
        triangle_ref.a = float3_fma(triangle_ref.a, radius, center);
        triangle_ref.b = float3_fma(triangle_ref.b, radius, center);
        triangle_ref.c = float3_fma(triangle_ref.c, radius, center);
    }
}

static void make_icosagedron(
    vector<triangle> &triangles,
    const flt radius,
    const float3 center
) {
    static const float3
        point_1  = { 0.0F, -1.0F / FLT_SQRT_PHI_2, FLT_PHI / FLT_SQRT_PHI_2 },
        point_2  = { 0.0F,  1.0 / FLT_SQRT_PHI_2,  FLT_PHI / FLT_SQRT_PHI_2 },
        point_3  = { -FLT_PHI / FLT_SQRT_PHI_2, 0.0, 1.0 / FLT_SQRT_PHI_2 },
        point_4  = { FLT_PHI / FLT_SQRT_PHI_2, 0.0, 1.0 / FLT_SQRT_PHI_2 },
        point_5  = { -1.0 / FLT_SQRT_PHI_2, FLT_PHI / FLT_SQRT_PHI_2, 0.0 },
        point_6  = { 1.0 / FLT_SQRT_PHI_2, FLT_PHI / FLT_SQRT_PHI_2, 0.0 },
        point_7  = { 1.0 / FLT_SQRT_PHI_2, -FLT_PHI / FLT_SQRT_PHI_2, 0.0 },
        point_8  = { -1.0 / FLT_SQRT_PHI_2, -FLT_PHI / FLT_SQRT_PHI_2, 0.0 },
        point_9  = { -FLT_PHI / FLT_SQRT_PHI_2, 0.0, -1.0 / FLT_SQRT_PHI_2 },
        point_10 = { FLT_PHI / FLT_SQRT_PHI_2, 0.0, -1.0 / FLT_SQRT_PHI_2 },
        point_11 = { 0.0, -1.0 / FLT_SQRT_PHI_2, -FLT_PHI / FLT_SQRT_PHI_2 },
        point_12 = { 0.0, 1.0 / FLT_SQRT_PHI_2, -FLT_PHI / FLT_SQRT_PHI_2 };

    triangles.push_back({ point_1, point_2, point_3, purple });
    triangles.push_back({ point_2, point_1, point_4, red });
    triangles.push_back({ point_1, point_3, point_8, yellow });
    triangles.push_back({ point_3, point_2, point_5, green });
    triangles.push_back({ point_5, point_2, point_6, darkgreen });
    triangles.push_back({ point_7, point_1, point_8, maroon });
    triangles.push_back({ point_4, point_1, point_7, purple });
    triangles.push_back({ point_2, point_4, point_6, red });
    triangles.push_back({ point_5, point_6, point_12, yellow });
    triangles.push_back({ point_7, point_8, point_11, green });
    triangles.push_back({ point_4, point_7, point_10, darkgreen });
    triangles.push_back({ point_6, point_4, point_10, maroon });
    triangles.push_back({ point_8, point_3, point_9, purple });
    triangles.push_back({ point_3, point_5, point_9, red });
    triangles.push_back({ point_10, point_11, point_12, yellow });
    triangles.push_back({ point_11, point_9, point_12, green });
    triangles.push_back({ point_6, point_10, point_12, darkgreen });
    triangles.push_back({ point_10, point_7, point_11, maroon });
    triangles.push_back({ point_8, point_9, point_11, purple });
    triangles.push_back({ point_9, point_5, point_12, red });

    for (size_t i = 0; i < 20U; ++i) {
        triangle &triangle_ref = triangles[triangles.size() - i - 1U];
        triangle_ref.a = float3_fma(triangle_ref.a, radius, center);
        triangle_ref.b = float3_fma(triangle_ref.b, radius, center);
        triangle_ref.c = float3_fma(triangle_ref.c, radius, center);
    }
}

static void make_tetrahedron(
    vector<triangle> &triangles,
    const flt radius,
    const float3 center
) {
    static const float3
        point_1 = { sqrtf(8.0F / 9.0F), 0.0F, -1.0F / 3.0F },
        point_2 = { -sqrtf(2.0F / 9.0F), sqrtf(2.0F / 3.0F), -1.0F / 3.0F },
        point_3 = { -sqrtf(2.0F / 9.0F), -sqrtf(2.0F / 3.0F), -1.0F / 3.0F },
        point_4 = { 0.0F, 0.0F, 1.0F };

    triangles.push_back({ point_2, point_1, point_3, gold });
    triangles.push_back({ point_1, point_2, point_4, darkred });
    triangles.push_back({ point_3, point_1, point_4, teal });
    triangles.push_back({ point_2, point_3, point_4, midnightblue });

    for (size_t i = 0; i < 4U; ++i) {
        triangle &triangle_ref = triangles[triangles.size() - i - 1U];
        triangle_ref.a = float3_fma(triangle_ref.a, radius, center);
        triangle_ref.b = float3_fma(triangle_ref.b, radius, center);
        triangle_ref.c = float3_fma(triangle_ref.c, radius, center);
    }
}

static void create_field(
    vector<triangle> &triangles,
    const float3 point_a,
    const float3 point_b,
    const float3 point_c,
    const float3 point_d,
    const float3 texture_color
) {
    const uchar4 color = {
        (uchar) (texture_color.x * UCHAR_MAX),
        (uchar) (texture_color.y * UCHAR_MAX),
        (uchar) (texture_color.z * UCHAR_MAX),
        0
    };

    triangles.push_back({ point_a, point_b, point_c, color });
    triangles.push_back({ point_a, point_c, point_d, color });
}

__device__ __host__ static uchar4 ray(
    float3 pos,
    float3 dir,
    const float3 sunshine,
    const float3 sunshine_c,
    triangle * const triangles,
    const size_t count
) {
    ptrdiff_t min_value = -1;
    flt ts_min;
    for (size_t i = 0; i < count; ++i) {
        const float3
            e1 = float3_minus(triangles[i].b, triangles[i].a),
            e2 = float3_minus(triangles[i].c, triangles[i].a),
            p = cross(dir, e2);
        const flt div = dot(p, e1);
        if (fabsf(div) < FLT_EPSILON)
            continue;
        const float3 t = float3_minus(pos, triangles[i].a);
        const flt u = dot(p, t) / div;
        if (u < 0.0F || u > 1.0F)
            continue;
        float3 q = cross(t, e1);
        flt v = dot(q, dir) / div;
        if (v < 0.0F || v + u > 1.0F)
            continue;
        flt ts = dot(q, e2) / div;
        if (ts < 0.0F)
            continue;
        if (min_value == -1 || ts < ts_min) {
            min_value = i;
            ts_min = ts;
        }
    }

    if (min_value == -1)
        return { 0, 0, 0, 0 };

    pos = float3_plus(float3_multiplies(dir, ts_min), pos);
    dir = float3_minus(sunshine, pos);
    const flt dir_length = length(dir);
    dir = normalize(dir);

    for (size_t i = 0; i < count; ++i) {
        const float3
            e1 = float3_minus(triangles[i].b, triangles[i].a),
            e2 = float3_minus(triangles[i].c, triangles[i].a),
            p = cross(dir, e2);
        const flt div = dot(p, e1);

        if (fabsf(div) < FLT_EPSILON)
            continue;
        float3 t = float3_minus(pos, triangles[i].a);
        const flt u = dot(p, t) / div;
        if (u < 0.0F || u > 1.0F)
            continue;
        float3 q = cross(t, e1);
        const flt v = dot(q, dir) / div;

        if (v < 0.0F || v + u > 1.0F)
            continue;

        const flt ts = dot(q, e2) / div;

        if (ts > 0.0F && ts < dir_length && i != min_value)
            return { 0, 0, 0, 0 };
    }

    const uchar4 color_min = triangles[min_value].color;
    const float3 result = float3_multiplies(
        make_float3(color_min.x, color_min.y, color_min.z),
        sunshine_c
    );
    return make_uchar4(result.x, result.y, result.z, color_min.w);
}

static void render_cpu(
    float3 pc, float3 pv,
    const uint w, const uint h,
    flt angle,
    uchar4 * const data,
    float3 sunshine, float3 sunshine_c,
    triangle * const triangles, const size_t count
) {
    const flt dw = 2.0F / (w - 1), dh = 2.0F / (h - 1),
        z = 1.0F / tanf(angle * FLT_PI / 360.0F);
    const float3
        bz = normalize(float3_minus(pv, pc)),
        bx = normalize(cross(bz, { 0.0F, 0.0F, 1.0F })),
        by = normalize(cross(bx, bz));
    for (uint i = 0; i < w; ++i)
        for (uint j = 0; j < h; ++j) {
            const float3 v = { -1.0F + dw * i, (-1.0F + dh * j) * h / w, z };
            const float3 dir = mult(bx, by, bz, v);
            data[(h - 1 - j) * w + i] = ray(pc, normalize(dir), sunshine, sunshine_c, triangles, count);
        }
}

__global__ static void render(
    const float3 pc, const float3 pv,
    const uint w, const uint h,
    const flt angle,
    uchar4 * const data,
    const float3 sunshine, const float3 sunshine_c,
    triangle * const triangles, const size_t count
) {
    const uint
        idx = blockDim.x * blockIdx.x + threadIdx.x,
        idy = blockDim.y * blockIdx.y + threadIdx.y,
        offsetx = blockDim.x * gridDim.x,
        offsety = blockDim.y * gridDim.y;

    const flt dw = 2.0F / (w - 1), dh = 2.0F / (h - 1),
        z = 1.0F / tanf(angle * FLT_PI / 360.0F);
    const float3
        bz = normalize(float3_minus(pv, pc)),
        bx = normalize(cross(bz, { 0.0F, 0.0F, 1.0F })),
        by = normalize(cross(bx, bz));
    for (uint i = idx; i < w; i += offsetx)
        for (uint j = idy; j < h; j += offsety) {
            const float3 v = { -1.0F + dw * i, (-1.0F + dh * j) * h / w, z },
                dir = mult(bx, by, bz, v);
            data[(h - 1 - j) * w + i] = ray(pc, normalize(dir), sunshine, sunshine_c, triangles, count);
        }
}

int main(const int argc, char ** const argv) {
    int flag_cuda, numbers_data, width, height, iter, light;
    float three_points_1, three_points_2, phi, r_first_0, z_first_0, f_first_0, A_first_r, A_first_z, w_first_r,
    w_first_z, w_first_f, p_first_r, p_first_z, r_second_0, z_second_0, f_second_0, A_second_r, A_second_z, w_second_r,
    w_second_z, w_second_f, p_second_r, p_second_z, rds, r_, trs, lght, tro;
    float3 cntr, clr, tp_1, tp_2, tp_3, tp_4, tc, pc, pv, sunshine, sunshine_c;
    string trash;
    char bffr[256];

    string exit_place;
    if (argc >= 3) {
        cout << "SOMETHING WENT WRONG......\nYOU CAN USE EQUAL OR LESS THEN 2 ARGUMENTS!\n";
        return 0;
    }
    if (argc == 1)
        flag_cuda = 1;
    else if (!strcmp(argv[1], "--gpu"))
        flag_cuda = 1;
    else if (!strcmp(argv[1], "--default")) {
        cout << "99"
            "./images_data/"
            "640 480 120\n"
            "6.8 2.7 0.1 2.2 1.1 2.1 6.1 1.1 0.0 0.0\n"
            "2.2 0.001 0.01 0.54 0.12 1.1 4.2 1.1 0.0 0.0\n"
            "3.0 0.0 0.0 255 255 255 1.0 42 42 42\n"
            "0.0 3.0 0.0 255 255 255 0.75 42 42 42\n"
            "0.0 0.0 0.0 255 255 255 1.0 42 42 42\n"
            "-5.0 -5.0 -1.0 -5.0 5.0 -1.0 5.0 5.0 -1.0 5.0 -5.0 -1.0 ~/textures 0.0 1.0 0.0 0.5\n"
            "1\n"
            "-8.0 0.0 8.0 1.0 1.0 1.0\n"
            "100 100\n";
        return 0;
    }
    else if (!strcmp(argv[1], "--cpu"))
        flag_cuda = 0;

    cin >> numbers_data >> exit_place >> width >> height >> phi;
    cin >> r_first_0 >> z_first_0 >> f_first_0 >> A_first_r >> A_first_z >> w_first_r >> w_first_z >> w_first_f >> p_first_r >> p_first_z >>
    r_second_0 >> z_second_0 >> f_second_0 >> A_second_r >> A_second_z >> w_second_r >> w_second_z >> w_second_f >> p_second_r >> p_second_z;

    uchar4 *data = (uchar4 *) malloc(width * height * sizeof(uchar4));
    uchar4 *deviceData;
    cudaMalloc(&deviceData, width * height * sizeof(uchar4));
    cudaMemset(deviceData, 0, width * height * sizeof(uchar4));

    vector <triangle> triangles;
    cin >> cntr.x >> cntr.y >> cntr.z >> clr.x >> clr.y >> clr.z >> rds >> r_ >> trs >> lght;
    make_tetrahedron(triangles, rds, cntr);

    cin >> cntr.x >> cntr.y >> cntr.z >> clr.x >> clr.y >> clr.z >> rds >> r_ >> trs >> lght;
    make_dodecahedron(triangles, rds, cntr);

    cin >> cntr.x >> cntr.y >> cntr.z >> clr.x >> clr.y >> clr.z >> rds >> r_ >> trs >> lght;
    make_icosagedron(triangles, rds, cntr);
    cin >> tp_1.x >> tp_1.y >> tp_1.z >> tp_2.x >> tp_2.y >> tp_2.z >> tp_3.x >> tp_3.y >> tp_3.z >> tp_4.x >> tp_4.y >> tp_4.z;
    cin >> trash >> tc.x >> tc.y >> tc.z >> tro;

    create_field(triangles, tp_1, tp_2, tp_3, tp_4, tc);
    cin >> light;
    if (light != 1) {
        cout << "I need in only 3 points for this project!\n";
        return 0;
    }

    cin >> sunshine.x >> sunshine.y >> sunshine.z >> sunshine_c.x >> sunshine_c.y >> sunshine_c.z >> three_points_1 >> three_points_2;
    sunshine.x = 100;
    sunshine.y = 100;
    sunshine.z = 100;

    triangle *deviceTriangles;
    cudaMalloc(&deviceTriangles, triangles.size() * sizeof(triangle));
    cudaMemcpy(deviceTriangles, triangles.data(), triangles.size() * sizeof(triangle),
        cudaMemcpyHostToDevice
    );

    iter = 0;
    while (iter < numbers_data) {
        float step = 2 * FLT_PI * iter / numbers_data;

        float r_first = A_first_r * sin(w_first_r * step + p_first_r) + r_first_0;
        float z_first = A_first_z * sin(w_first_z * step + p_first_z) + z_first_0;
        float f_first = w_first_f * step + f_first_0;

        float r_second = A_second_r * sin(w_second_r * step + p_second_r) + r_second_0;
        float z_second = A_second_z * sin(w_second_z * step + p_second_z) + z_second_0;
        float f_second = w_second_f * step + f_second_0;

        pc.x = cos(f_first) * r_first;
        pc.y = sin(f_first) * r_first;
        pc.z = z_first;

        pv.x = cos(f_second) * r_second;
        pv.y = sin(f_second) * r_second;
        pv.z = z_second;

        if (flag_cuda == 1) {
            render<<<dim3(16, 16), dim3(16, 16)>>>(pc, pv, width, height, phi, deviceData, sunshine, sunshine_c, deviceTriangles, triangles.size());
            cudaMemcpy(data, deviceData, sizeof(uchar4) * width * height, cudaMemcpyDeviceToHost);
        }
        else
            render_cpu(pc, pv, width, height, phi, data, sunshine, sunshine_c, triangles.data(), triangles.size());


        sprintf(bffr, "%d.data", iter);
        const string result_show(exit_place + "/" + string(bffr));

        cout << iter << ": " << bffr << endl;
        FILE* out = fopen(result_show.c_str(), "wb");
        fwrite(&width, sizeof(int), 1, out);
        fwrite(&height, sizeof(int), 1, out);
        fwrite(data, sizeof(uchar4), width * height, out);
        fclose(out);

        ++iter;
    }

    cudaFree(deviceData);
    cudaFree(deviceTriangles);
    free(data);

    return 0;
}
