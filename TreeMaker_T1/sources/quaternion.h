/*
 * quaternion.h
 *
 * Original Author:
 *     Will Perone
 *
 * Edited By:
 *      Owen Patrick
 *
 * Description:
 *      A standard quaternion class provided under a public liscence.
 */
#ifndef QUATERNION_H
#define QUATERNION_H

#include "vecmath.h"

class quaternion
{

public:
    //! constructors
    quaternion() {}
    quaternion(float real, float x, float y, float z): s(real), v(x,y,z) {}
    quaternion(float real, const Vector3 &i): s(real), v(i) {}

    //! from 3 euler angles
    quaternion(float theta_z, float theta_y, float theta_x)
    {
        float cos_z_2 = cosf(0.5*theta_z);
        float cos_y_2 = cosf(0.5*theta_y);
        float cos_x_2 = cosf(0.5*theta_x);

        float sin_z_2 = sinf(0.5*theta_z);
        float sin_y_2 = sinf(0.5*theta_y);
        float sin_x_2 = sinf(0.5*theta_x);

        s   = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
        v.x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
        v.y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
        v.z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;

    }

    //! from 3 euler angles
    quaternion(const Vector3 &angles)
    {
        float cos_z_2 = cosf(0.5*angles.z);
        float cos_y_2 = cosf(0.5*angles.y);
        float cos_x_2 = cosf(0.5*angles.x);

        float sin_z_2 = sinf(0.5*angles.z);
        float sin_y_2 = sinf(0.5*angles.y);
        float sin_x_2 = sinf(0.5*angles.x);

        s   = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
        v.x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
        v.y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
        v.z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;
    }

    //! basic operations
    quaternion &operator =(const quaternion &q)
    { s= q.s; v= q.v; return *this; }

    const quaternion operator +(const quaternion &q) const
    { return quaternion(s+q.s, v+q.v); }

    const quaternion operator -(const quaternion &q) const
    { return quaternion(s-q.s, v-q.v); }

    const quaternion operator *(const quaternion &q) const
    {	return quaternion(s*q.s - v*q.v,
                  v.y*q.v.z - v.z*q.v.y + s*q.v.x + v.x*q.s,
                  v.z*q.v.x - v.x*q.v.z + s*q.v.y + v.y*q.s,
                  v.x*q.v.y - v.y*q.v.x + s*q.v.z + v.z*q.s);
    }

    const quaternion operator /(const quaternion &q) const
    {
        quaternion p(q);
        p.invert();
        return *this * p;
    }

    const quaternion operator *(float scale) const
    { return quaternion(s*scale,v*scale); }

    const quaternion operator /(float scale) const
    { return quaternion(s/scale,v/scale); }

    const quaternion operator -() const
    { return quaternion(-s, -v); }

    const quaternion &operator +=(const quaternion &q)
    { v+=q.v; s+=q.s; return *this; }

    const quaternion &operator -=(const quaternion &q)
    { v-=q.v; s-=q.s; return *this; }

    const quaternion &operator *=(const quaternion &q)
    {
        float x= v.x, y= v.y, z= v.z, sn= s*q.s - v*q.v;
        v.x= y*q.v.z - z*q.v.y + s*q.v.x + x*q.s;
        v.y= z*q.v.x - x*q.v.z + s*q.v.y + y*q.s;
        v.z= x*q.v.y - y*q.v.x + s*q.v.z + z*q.s;
        s= sn;
        return *this;
    }

    const quaternion &operator *= (float scale)
    { v*=scale; s*=scale; return *this; }

    const quaternion &operator /= (float scale)
    { v/=scale; s/=scale; return *this; }


    //! gets the length of this quaternion
    float length() const
    { return (float)sqrt(s*s + v*v); }

    //! gets the squared length of this quaternion
    float length_squared() const
    { return (float)(s*s + v*v); }

    //! normalizes this quaternion
    void normalize()
    { *this/=length(); }

    //! returns the normalized version of this quaternion
    quaternion normalized() const
    { return  *this/length(); }

    //! computes the conjugate of this quaternion
    void conjugate()
    { v=-v; }

    //! inverts this quaternion
    void invert()
    { conjugate(); *this/=length_squared(); }

    //! returns the logarithm of a quaternion = v*a where q = [cos(a),v*sin(a)]
    quaternion log() const
    {
        float a = (float)acos(s);
        float sina = (float)sin(a);
        quaternion ret;

        ret.s = 0;
        if (sina > 0)
        {
            ret.v.x = a*v.x/sina;
            ret.v.y = a*v.y/sina;
            ret.v.z = a*v.z/sina;
        } else {
            ret.v.x= ret.v.y= ret.v.z= 0;
        }
        return ret;
    }

    //! returns e^quaternion = exp(v*a) = [cos(a),vsin(a)]
    quaternion exp() const
    {
        float a = (float)v.length();
        float sina = (float)sin(a);
        float cosa = (float)cos(a);
        quaternion ret;

        ret.s = cosa;
        if (a > 0)
        {
            ret.v.x = sina * v.x / a;
            ret.v.y = sina * v.y / a;
            ret.v.z = sina * v.z / a;
        } else {
            ret.v.x = ret.v.y = ret.v.z = 0;
        }
        return ret;
    }


    //! computes the dot product of 2 quaternions
    static inline float dot(const quaternion &q1, const quaternion &q2)
    { return q1.v*q2.v + q1.s*q2.s; }

    //! linear quaternion interpolation
    static quaternion lerp(const quaternion &q1, const quaternion &q2, float t)
    { return (q1*(1-t) + q2*t).normalized(); }

    //! spherical linear interpolation
    static quaternion slerp(const quaternion &q1, const quaternion &q2, float t)
    {
        quaternion q3;
        float dot = quaternion::dot(q1, q2);

        if (dot < 0)
        {
            dot = -dot;
            q3 = -q2;
        } else q3 = q2;

        if (dot < 0.95f)
        {
            float angle = acosf(dot);
            return (q1*sinf(angle*(1-t)) + q3*sinf(angle*t))/sinf(angle);
        } else
            return lerp(q1,q3,t);

    }

    //! This version of slerp, used by squad, does not check for theta > 90.
    static quaternion slerpNoInvert(const quaternion &q1, const quaternion &q2, float t)
    {
        float dot = quaternion::dot(q1, q2);

        if (dot > -0.95f && dot < 0.95f)
        {
            float angle = acosf(dot);
            return (q1*sinf(angle*(1-t)) + q2*sinf(angle*t))/sinf(angle);
        } else
            return lerp(q1,q2,t);
    }

    //! spherical cubic interpolation
    static quaternion squad(const quaternion &q1,const quaternion &q2,const quaternion &a,const quaternion &b,float t)
    {
        quaternion c= slerpNoInvert(q1,q2,t),
                   d= slerpNoInvert(a,b,t);
        return slerpNoInvert(c,d,2*t*(1-t));
    }

    //! Shoemake-Bezier interpolation using De Castlejau algorithm
    static quaternion bezier(const quaternion &q1,const quaternion &q2,const quaternion &a,const quaternion &b,float t)
    {
        quaternion q11= slerpNoInvert(q1,a,t),
                q12= slerpNoInvert(a,b,t),
                q13= slerpNoInvert(b,q2,t);
        return slerpNoInvert(slerpNoInvert(q11,q12,t), slerpNoInvert(q12,q13,t), t);
    }

    //! Given 3 quaternions, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
    static quaternion spline(const quaternion &qnm1,const quaternion &qn,const quaternion &qnp1)
    {
        quaternion qni(qn.s, -qn.v);
        return qn * (( (qni*qnm1).log()+(qni*qnp1).log() )/-4).exp();
    }

    //! converts from a normalized axis - angle pair rotation to a quaternion
    static inline quaternion from_axis_angle(const Vector3 &axis, float angle)
    { return quaternion(cosf((angle/2) * M_PI/180), axis*sinf((angle/2) * M_PI/180)); }

    //! returns the axis and angle of this unit quaternion
    void to_axis_angle(Vector3 &axis, float &angle) const
    {
        angle = acosf(s);

        // pre-compute to save time
        float sinf_theta_inv = 1.0/sinf(angle);

        // now the vector
        axis.x = v.x*sinf_theta_inv;
        axis.y = v.y*sinf_theta_inv;
        axis.z = v.z*sinf_theta_inv;

        // multiply by 2
        angle*=2;
    }

    //! rotates v by this quaternion (quaternion must be unit)
    Vector3 rotate(const Vector3 &v)
    {
        quaternion V(0, v);
        quaternion conjugate(*this);
        conjugate.conjugate();
        return (*this * V * conjugate).v;
    }

    //! returns the euler angles from a rotation quaternion
    Vector3 euler_angles(bool homogenous=true) const
    {
        float sqw = s*s;
        float sqx = v.x*v.x;
        float sqy = v.y*v.y;
        float sqz = v.z*v.z;

        Vector3 euler;
        if (homogenous) {
            euler.x = atan2f(2.f * (v.x*v.y + v.z*s), sqx - sqy - sqz + sqw) * 180/M_PI;
            euler.y = asinf(-2.f * (v.x*v.z - v.y*s)) * 180/M_PI;
            euler.z = atan2f(2.f * (v.y*v.z + v.x*s), -sqx - sqy + sqz + sqw) * 180/M_PI;
        } else {
            euler.x = atan2f(2.f * (v.z*v.y + v.x*s), 1 - 2*(sqx + sqy)) * 180/M_PI;
            euler.y = asinf(-2.f * (v.x*v.z - v.y*s)) * 180/M_PI;
            euler.z = atan2f(2.f * (v.x*v.y + v.z*s), 1 - 2*(sqy + sqz)) * 180/M_PI;
        }

//        euler.x = atan2f(2.f * (v.y*s - v.x*v.z), 1 - 2*(sqy - sqz));
//        euler.y = asinf(2.f * (v.x*v.y + v.z*s));
//        euler.z = atan2f(2.f * (v.x*s - v.y*v.z), 1 - 2*(sqx - sqz));

        return euler;
    }

    float   s; //!< the real component
    Vector3 v; //!< the imaginary components
};

#endif
