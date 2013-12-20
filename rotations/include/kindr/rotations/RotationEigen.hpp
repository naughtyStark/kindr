/*
 * Copyright (c) 2013, Christian Gehring, Hannes Sommer, Paul Furgale, Remo Diethelm
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autonomous Systems Lab, ETH Zurich nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Christian Gehring, Hannes Sommer, Paul Furgale,
 * Remo Diethelm BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#ifndef KINDR_ROTATIONS_ROTATIONEIGEN_HPP_
#define KINDR_ROTATIONS_ROTATIONEIGEN_HPP_

#include "kindr/rotations/RotationBase.hpp"
#include "kindr/positions/PositionEigen.hpp"

namespace kindr {
namespace rotations {
namespace eigen_impl {

template<typename PrimType_, enum RotationUsage Usage_>
class AngleAxis;

template<typename PrimType_, enum RotationUsage Usage_>
class RotationVector;

template<typename PrimType_, enum RotationUsage Usage_>
class RotationQuaternion;

template<typename PrimType_, enum RotationUsage Usage_>
class RotationMatrix;

template<typename PrimType_, enum RotationUsage Usage_>
class EulerAnglesZyx;

template<typename PrimType_, enum RotationUsage Usage_>
class EulerAnglesXyz;


} // namespace eigen_impl


namespace internal {

template<typename PrimType_>
class get_position3<positions::eigen_impl::Position3<PrimType_>>{
 private:
  typedef typename positions::eigen_impl::Position3<PrimType_> Position;
  typedef typename Position::Implementation Matrix3X;
 public:
  static const Matrix3X& getMatrix3(const Position& position) {
    return position.toImplementation();
  }

};


/*! \brief Multiplication of two rotations with different parameterizations
 */
template<typename Left_, typename Right_, enum RotationUsage Usage_>
class MultiplicationTraits<RotationBase<Left_, Usage_>, RotationBase<Right_, Usage_>> {
 public:
  //! Default multiplication of rotations converts the representations of the rotations to rotation quaternions and multiplies them
  inline static Left_ mult(const RotationBase<Left_, Usage_>& lhs, const RotationBase<Right_, Usage_>& rhs) {
    return Left_(typename eigen_impl::RotationQuaternion<typename Left_::Scalar,  Usage_>(
               (typename eigen_impl::RotationQuaternion<typename Left_::Scalar,  Usage_>(lhs.derived())).toImplementation() *
               (typename eigen_impl::RotationQuaternion<typename Right_::Scalar, Usage_>(rhs.derived())).toImplementation()
               ));
  }
};

/*! \brief Multiplication of two rotations with the same parameterization
 */
template<typename LeftAndRight_, enum RotationUsage Usage_>
class MultiplicationTraits<RotationBase<LeftAndRight_, Usage_>, RotationBase<LeftAndRight_, Usage_>> {
 public:
  inline static LeftAndRight_ mult(const RotationBase<LeftAndRight_, Usage_>& lhs, const RotationBase<LeftAndRight_, Usage_>& rhs) {
    if(Usage_ == RotationUsage::ACTIVE){
      return LeftAndRight_(typename LeftAndRight_::Implementation(lhs.derived().toImplementation() * rhs.derived().toImplementation()));
    } else {
      return LeftAndRight_(typename LeftAndRight_::Implementation(rhs.derived().toImplementation() * lhs.derived().toImplementation()));
    }
  }
};


template<typename Left_, typename Right_, enum RotationUsage Usage_>
class ComparisonTraits<RotationBase<Left_, Usage_>, RotationBase<Right_, Usage_>> {
 public:
  inline static double getDisparityAngle(const Left_ & left, const Right_ & right) {
    return (typename eigen_impl::AngleAxis<typename Left_::Scalar,  Usage_>(left*right.inverse())).angle();
  }
};


} // namespace internal




} // namespace rotations
} // namespace kindr


#include "kindr/rotations/eigen/AngleAxis.hpp"
#include "kindr/rotations/eigen/RotationVector.hpp"
#include "kindr/rotations/eigen/RotationQuaternion.hpp"
#include "kindr/rotations/eigen/RotationMatrix.hpp"
#include "kindr/rotations/eigen/EulerAnglesZyx.hpp"
#include "kindr/rotations/eigen/EulerAnglesXyz.hpp"




#endif /* KINDR_ROTATIONS_ROTATIONEIGEN_HPP_ */
