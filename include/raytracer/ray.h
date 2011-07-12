#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "raytracer/vector.h"

namespace Raytracer {

template<class T = float, size_t N = 3>
class Ray
{
    public:
        Ray(): m_origin(), m_direction()
        {
        }

        /** Default constructor */
        Ray(Vector<T, N> origin, Vector<T, N> direction): m_origin(origin), m_direction(direction)
        {
            m_direction.normalise();
        }

        /** Access m_origin
         * \return The current value of m_origin
         */
        Vector<T, N> &origin() { return m_origin; }
        /** Set m_origin
         * \param val New value to set
         */
        Vector<T, N> &direction() { return m_direction; }
        /** Set m_direction
         * \param val New value to set
         */
    protected:
    private:
        Vector<T, N> m_origin; //!< Member variable "m_origin"
        Vector<T, N> m_direction; //!< Member variable "m_direction"
};

} // namespace Raytracer

#endif // RAYTRACER_RAY_H
