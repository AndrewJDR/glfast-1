#include "flycamera.h"

void flycamera(
    float * cam_vec3, float * cam_quat, float * cam_m3x3, float look_mult,
    float move_mult, float x_delta, float y_delta, float key_f, float key_l,
    float key_b, float key_r, float key_u, float key_d)
{
  // y_quat = quatFromAxisAngle({1, 0, 0}, y_delta * look_mult *
  // to_radians)
  // x_quat = quatFromAxisAngle({0, 1, 0}, x_delta * look_mult *
  // to_radians)

  const float ydt = y_delta * look_mult * 0.017453f;
  const float xdt = x_delta * look_mult * 0.017453f;
  const float yqx = (float)sin(ydt);
  const float yqw = (float)cos(ydt);
  const float xqy = (float)sin(xdt);
  const float xqw = (float)cos(xdt);

  // cam_quat = cam_quat * y_quat
  // cam_quat = x_quat * cam_quat

  const float iqx = cam_quat[0];
  const float iqy = cam_quat[1];
  const float iqz = cam_quat[2];
  const float iqw = cam_quat[3];

  const float tqx = +iqw * yqx + iqx * yqw;
  const float tqy = -iqz * yqx - iqy * yqw;
  const float tqz = -iqy * yqx + iqz * yqw;
  const float tqw = +iqx * yqx - iqw * yqw;

  const float cqx = +tqz * xqy + tqx * xqw;
  const float cqy = -tqw * xqy - tqy * xqw;
  const float cqz = -tqx * xqy + tqz * xqw;
  const float cqw = +tqy * xqy - tqw * xqw;

  // x_axis = rotateVec3ByQuat({-1, 0, 0}, cam_quat)
  // y_axis = rotateVec3ByQuat({ 0, 1, 0}, cam_quat)
  // z_axis = rotateVec3ByQuat({ 0, 0,-1}, cam_quat)

  const float qxqx = cqx * cqx;
  const float qxqy = cqx * cqy;
  const float qxqz = cqx * cqz;
  const float qxqw = cqx * cqw;

  const float qyqy = cqy * cqy;
  const float qyqz = cqy * cqz;
  const float qyqw = cqy * cqw;

  const float qzqz = cqz * cqz;
  const float qzqw = cqz * cqw;

  const float xax = (qyqy + qzqz) * +2.f - 1.f;
  const float xay = (qxqy + qzqw) * -2.f;
  const float xaz = (qyqw - qxqz) * +2.f;

  const float yax = (qxqy - qzqw) * +2.f;
  const float yay = (qxqx + qzqz) * -2.f + 1.f;
  const float yaz = (qxqw + qyqz) * +2.f;

  const float zax = (qyqw + qxqz) * -2.f;
  const float zay = (qxqw - qyqz) * +2.f;
  const float zaz = (qxqx + qyqy) * +2.f - 1.f;

  const float xm = key_l - key_r;
  const float ym = key_u - key_d;
  const float zm = key_f - key_b;

  float mx = xax * xm + yax * ym + zax * zm;
  float my = xay * xm + yay * ym + zay * zm;
  float mz = xaz * xm + yaz * ym + zaz * zm;

  const float ln = (float)sqrt(mx * mx + my * my + mz * mz);
  if (ln != 0.f)
  {
    mx /= ln;
    my /= ln;
    mz /= ln;
  }

  cam_vec3[0] += mx * move_mult;
  cam_vec3[1] += my * move_mult;
  cam_vec3[2] += mz * move_mult;

  cam_quat[0] = cqx;
  cam_quat[1] = cqy;
  cam_quat[2] = cqz;
  cam_quat[3] = cqw;

  cam_m3x3[0] = -xax;
  cam_m3x3[1] = +yax;
  cam_m3x3[2] = -zax;

  cam_m3x3[3] = -xay;
  cam_m3x3[4] = +yay;
  cam_m3x3[5] = -zay;

  cam_m3x3[6] = -xaz;
  cam_m3x3[7] = +yaz;
  cam_m3x3[8] = -zaz;
}
