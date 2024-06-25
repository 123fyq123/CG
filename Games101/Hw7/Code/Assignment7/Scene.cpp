//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"

void Scene::buildBVH()
{
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

// void Scene::sampleLight(Intersection &pos, float &pdf) const
// {
//     float emit_area_sum = 0;
//     for (uint32_t k = 0; k < objects.size(); ++k)
//     {
//         if (objects[k]->hasEmit())
//         {
//             emit_area_sum += objects[k]->getArea();
//         }
//     }
//     float p = get_random_float() * emit_area_sum;
//     emit_area_sum = 0;
//     for (uint32_t k = 0; k < objects.size(); ++k)
//     {
//         if (objects[k]->hasEmit())
//         {
//             emit_area_sum += objects[k]->getArea();
//             if (p <= emit_area_sum)
//             {
//                 objects[k]->Sample(pos, pdf);
//                 break;
//             }
//         }
//     }
// }

// 实现了采样光源的接口
// 对场景中的光源进行随机采样，以pdf进行
void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        if (objects[k]->hasEmit())
        {                                           // 第k个物体有自发光，hasEmit ->bool量
            emit_area_sum += objects[k]->getArea(); // 得到场景中自发光区域的面积和，用以后续求pdf=1/area
        }
    }
    // 对场景中的所有光源按面积均匀采样一个点，计算
    float p = get_random_float() * emit_area_sum; // 随机取[0, emit_area_sum]之间的浮点数
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        if (objects[k]->hasEmit())
        {
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum)
            { // 随机选取一个光源面，即第k个自发光物体的光源面
                // 利用Sample()在光源面中按照pdf的概率随即找到一个点pos，得到这个点pos的信息
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}
bool Scene::trace(
    const Ray &ray,
    const std::vector<Object *> &objects,
    float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear)
        {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }

    return (*hitObject != nullptr);
}

// // Implementation of Path Tracing
// Vector3f Scene::castRay(const Ray &ray, int depth) const
// {
//     // 创建变量以储存直接和间接光照计算值
//     Vector3f dir = {0.0, 0.0, 0.0};
//     Vector3f indir = {0.0, 0.0, 0.0};
//     // 1.判断是否有交点：光线与场景中物体相交？
//     Intersection inter = Scene::intersect(ray);
//     // 如果没交点
//     if (!inter.happened)
//     {
//         return dir; // return 0,0,0
//     }
//     // 2.ray打到光源了：说明渲染方程只用算前面的自发光项，因此直接返回材质的自发光项
//     if (inter.m->hasEmission())
//     {
//         if (depth == 0)
//         { // 第一次打到光
//             return inter.m->getEmission();
//         }
//         else
//             return dir; // 弹射打到光，直接返回0，0.0
//     }
//     // 3.ray打到物体：这个时候才开始进行伪代码后面的步骤

//     // 对场景中的光源进行采样，得到采样点light_pos和pdf_light
//     Intersection light_pos;
//     float pdf_light = 0.0f;
//     sampleLight(light_pos, pdf_light);

//     // 3.1计算直接光照

//     // 物体的一些参数
//     Vector3f p = inter.coords;
//     Vector3f N = inter.normal.normalized();
//     Vector3f wo = ray.direction; // 物体指向场景
//     // 光源的一些参数
//     Vector3f xx = light_pos.coords;
//     Vector3f NN = light_pos.normal.normalized();
//     Vector3f ws = (p - xx).normalized(); // 光源指向物体
//     float dis = (p - xx).norm();         // 二者距离
//     float dis2 = dotProduct((p - xx), (p - xx));

//     // 判断光源与物体间是否有遮挡：
//     // 发出一条射线，方向为ws 光源xx -> 物体p
//     Ray light_to_obj(xx, ws); // Ray(orig,dir)
//     Intersection light_to_scene = Scene::intersect(light_to_obj);
//     // 假如dis>light_to_scene.distance就说明有遮挡，那么反着给条件即可：
//     if (light_to_scene.happened && (light_to_scene.distance - dis > -EPSILON))
//     { // 没有遮挡
//         // 为了更贴近伪代码，先设定一些参数
//         Vector3f L_i = light_pos.emit;            // 光强
//         Vector3f f_r = inter.m->eval(wo, -ws, N); // 材质，课上说了，BRDF==材质，ws不参与计算
//         float cos_theta = dotProduct(-ws, N);     // 物体夹角
//         float cos_theta_l = dotProduct(ws, NN);   // 光源夹角
//         dir = L_i * f_r * cos_theta * cos_theta_l / dis2 / pdf_light;
//     }

//     // 3.2间接光照

//     // 俄罗斯轮盘赌
//     // Scene.hpp中已经定义了P_RR:RussianRoulette=0.8
//     float ksi = get_random_float(); // 随机取[0,1]
//     if (ksi < RussianRoulette)
//     {
//         // 计算间接光照

//         // 随机生成一个wi方向
//         Vector3f wi = inter.m->sample(wo, N).normalized(); // 这里的wi其实没参与计算，返回的是一个随机的方向
//         Ray r(p, wi);
//         Intersection obj_to_scene = Scene::intersect(r);
//         // 击中了物体&&物体不是光源
//         if (obj_to_scene.happened && !obj_to_scene.m->hasEmission())
//         {
//             Vector3f f_r = inter.m->eval(wo, wi, N); // wo不参与计算
//             float cos_theta = dotProduct(wi, N);
//             float pdf_hemi = inter.m->pdf(wo, wi, N);
//             indir = castRay(r, depth + 1) * f_r * cos_theta / pdf_hemi / RussianRoulette;
//         }
//     }
//     return dir + indir;
// }
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Vector3f hitColor = this->backgroundColor;
    Intersection shade_point_inter = Scene::intersect(ray);
    if (shade_point_inter.happened)
    {

        Vector3f p = shade_point_inter.coords;
        Vector3f wo = ray.direction;
        Vector3f N = shade_point_inter.normal;
        Vector3f L_dir(0), L_indir(0);

        // sampleLight(inter,pdf_light)
        Intersection light_point_inter;
        float pdf_light;
        sampleLight(light_point_inter, pdf_light);
        // Get x,ws,NN,emit from inter
        Vector3f x = light_point_inter.coords;
        Vector3f ws = normalize(x - p);
        Vector3f NN = light_point_inter.normal;
        Vector3f emit = light_point_inter.emit;
        float distance_pTox = (x - p).norm();
        // Shoot a ray from p to x
        Vector3f p_deviation = (dotProduct(ray.direction, N) < 0) ? p + N * EPSILON : p - N * EPSILON;

        Ray ray_pTox(p_deviation, ws);
        // If the ray is not blocked in the middleff
        Intersection blocked_point_inter = Scene::intersect(ray_pTox);
        if (abs(distance_pTox - blocked_point_inter.distance < 0.01))
        {
            L_dir = emit * shade_point_inter.m->eval(wo, ws, N) * dotProduct(ws, N) * dotProduct(-ws, NN) / (distance_pTox * distance_pTox * pdf_light);
        }
        // Test Russian Roulette with probability RussianRouolette
        float ksi = get_random_float();
        if (ksi < RussianRoulette)
        {
            // wi=sample(wo,N)
            Vector3f wi = normalize(shade_point_inter.m->sample(wo, N));
            // Trace a ray r(p,wi)
            Ray ray_pTowi(p_deviation, wi);
            // If ray r hit a non-emitting object at q
            Intersection bounce_point_inter = Scene::intersect(ray_pTowi);
            if (bounce_point_inter.happened && !bounce_point_inter.m->hasEmission())
            {
                float pdf = shade_point_inter.m->pdf(wo, wi, N);
                if (pdf > EPSILON)
                    L_indir = castRay(ray_pTowi, depth + 1) * shade_point_inter.m->eval(wo, wi, N) * dotProduct(wi, N) / (pdf * RussianRoulette);
            }
        }
        hitColor = shade_point_inter.m->getEmission() + L_dir + L_indir;
    }
    return hitColor;
}
