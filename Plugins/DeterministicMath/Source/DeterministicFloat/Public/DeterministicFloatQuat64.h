// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DeterministicFloatFwd.h"
#include "DeterministicFloatNumbers.h"
#include "DeterministicFloatQuat64.generated.h"

USTRUCT(BlueprintType)
struct DETERMINISTICFLOAT_API FDeterministicQuat64
{
public:
	GENERATED_BODY()

		/**
		* Default constructor, no initialization.
		*/
		FORCEINLINE FDeterministicQuat64();

	/**
	* Constructor that casts from FQuat to FDeterministicQuat64
	*/
	FORCEINLINE FDeterministicQuat64(FQuat Other);

	/**
	 * Creates and initializes a new quaternion, with the W component either 0 or 1.
	 *
	 * @param EForceInit Force init enum: if equal to ForceInitToZero then W is 0, otherwise W = 1 (creating an identity transform)
	 */
	explicit FORCEINLINE FDeterministicQuat64(EForceInit ZeroOrNot);

	FORCEINLINE FDeterministicQuat64(const FDeterministicVector64& inAxis, const FDeterministicNumber64& inAngleRad);

	FORCEINLINE FDeterministicQuat64(const FDeterministicNumber64& inValue);

	FORCEINLINE FDeterministicQuat64(const FDeterministicNumber64& inX, const FDeterministicNumber64& inY, const FDeterministicNumber64& inZ, const FDeterministicNumber64& inW);

	/**
	 * Creates and initializes a new quaternion from the given rotator.
	 *
	 * @param R The rotator to initialize from.
	 */
	explicit FDeterministicQuat64(const FDeterministicRotator64& R);

	/**
	 * Creates and initializes a new quaternion from the given matrix.
	 *
	 * @param M The rotation matrix to initialize from.
	 */
	explicit FDeterministicQuat64(const FDeterministicMatrix64& M);

	UPROPERTY(EditAnywhere)
	FDeterministicNumber64 X;

	UPROPERTY(EditAnywhere)
	FDeterministicNumber64 Y;

	UPROPERTY(EditAnywhere)
	FDeterministicNumber64 Z;

	UPROPERTY(EditAnywhere)
	FDeterministicNumber64 W;

	static const FDeterministicQuat64 Identity;

	/**
	 * Gets the result of adding a Quaternion to this.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to add.
	 * @return The result of addition.
	 */
	FORCEINLINE FDeterministicQuat64 operator+(const FDeterministicQuat64& Q) const;

	/**
	 * Adds to this quaternion.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Other The quaternion to add to this.
	 * @return Result after addition.
	 */
	FORCEINLINE FDeterministicQuat64 operator+=(const FDeterministicQuat64& Q);

	/**
	 * Gets the result of subtracting a Quaternion to this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to subtract.
	 * @return The result of subtraction.
	 */
	FORCEINLINE FDeterministicQuat64 operator-(const FDeterministicQuat64& Q) const;

	/**
	* Negates the quaternion. Note that this represents the same rotation.
	*
	* @return The result of negation.
	*/
	FORCEINLINE FDeterministicQuat64 operator-() const;

	/**
	 * Checks whether another Quaternion is equal to this, within specified tolerance.
	 *
	 * @param Q The other Quaternion.
	 * @param Tolerance Error tolerance for comparison with other Quaternion.
	 * @return true if two Quaternions are equal, within specified tolerance, otherwise false.
	 */
	FORCEINLINE bool Equals(const FDeterministicQuat64& Q, FDeterministicNumber64 Tolerance = DeterministicFloat::Constants::Number64::KindaSmallNumber) const;

	/**
	 * Checks whether this Quaternion is an Identity Quaternion.
	 * Assumes Quaternion tested is normalized.
	 *
	 * @param Tolerance Error tolerance for comparison with Identity Quaternion.
	 * @return true if Quaternion is a normalized Identity Quaternion.
	 */
	FORCEINLINE bool IsIdentity(FDeterministicNumber64 Tolerance = DeterministicFloat::Constants::Number64::SmallNumber) const;

	/**
	 * Subtracts another quaternion from this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param Q The other quaternion.
	 * @return reference to this after subtraction.
	 */
	FORCEINLINE FDeterministicQuat64 operator-=(const FDeterministicQuat64& Q);

	/**
	 * Gets the result of multiplying this by another quaternion (this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q The Quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
	FORCEINLINE FDeterministicQuat64 operator*(const FDeterministicQuat64& Q) const;

	/**
	 * Multiply this by a quaternion (this = this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q the quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
	FORCEINLINE FDeterministicQuat64 operator*=(const FDeterministicQuat64& Q);

	/**
	 * Rotate a vector by this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation
	 * @see RotateVector
	 */
	FDeterministicVector64 operator*(const FDeterministicVector64& V) const;

	/**
	 * Multiply this by a matrix.
	 * This matrix conversion came from
	 * http://www.m-hikari.com/ija/ija-password-2008/ija-password17-20-2008/aristidouIJA17-20-2008.pdf
	 * used for non-uniform scaling transform.
	 *
	 * @param M Matrix to multiply by.
	 * @return Matrix result after multiplication.
	 */
	FDeterministicMatrix64 operator*(const FDeterministicMatrix64& M) const;

	FORCEINLINE FDeterministicQuat64 operator*=(const FDeterministicNumber64& Scale);

	/**
	 * Get the result of scaling this quaternion.
	 *
	 * @param Scale The scaling factor.
	 * @return The result of scaling.
	 */
	FORCEINLINE FDeterministicQuat64 operator*(const FDeterministicNumber64& Scale) const;

	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return a reference to this after scaling.
	 */
	FORCEINLINE FDeterministicQuat64 operator/=(const FDeterministicNumber64& Scale);

	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return new Quaternion of this after division by scale.
	 */
	FORCEINLINE FDeterministicQuat64 operator/(const FDeterministicNumber64& Scale) const;

	/**
	 * Identical implementation for TQuat properties.
	 * Avoids intrinsics to remain consistent with previous per-property comparison.
	 */
	bool Identical(const FDeterministicQuat64* Q, const uint32 PortFlags) const;

	/**
	 * Checks whether two quaternions are identical.
	 * This is an exact comparison per-component;see Equals() for a comparison
	 * that allows for a small error tolerance and flipped axes of rotation.
	 *
	 * @param Q The other quaternion.
	 * @return true if two quaternion are identical, otherwise false.
	 * @see Equals
	 */
	bool operator==(const FDeterministicQuat64& Q) const;

	/**
	 * Checks whether two quaternions are not identical.
	 *
	 * @param Q The other quaternion.
	 * @return true if two quaternion are not identical, otherwise false.
	 */
	bool operator!=(const FDeterministicQuat64& Q) const;

	/**
	 * Calculates dot product of two quaternions.
	 *
	 * @param Q The other quaternions.
	 * @return The dot product.
	 */
	FDeterministicNumber64 operator|(const FDeterministicQuat64& Q) const;

	/**
	 * Convert a vector of floating-point Euler angles (in degrees) into a Quaternion.
	 *
	 * @param Euler the Euler angles
	 * @return constructed TQuat
	 */
	static FDeterministicQuat64 MakeFromEuler(const FDeterministicVector64& Euler);

	/** Convert a Quaternion into floating-point Euler angles (in degrees). */
	FDeterministicVector64 Euler() const;

	/**
	 * Get the axis of rotation of the Quaternion.
	 * This is the axis around which rotation occurs to transform the canonical coordinate system to the target orientation.
	 * For the identity Quaternion which has no such rotation, TVector<T>(1,0,0) is returned.
	 */
	FORCEINLINE FDeterministicVector64 GetRotationAxis() const;

	/**
	 * Normalize this quaternion if it is large enough.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param Tolerance Minimum squared length of quaternion for normalization.
	 */
	FORCEINLINE void Normalize(FDeterministicNumber64 Tolerance = DeterministicFloat::Constants::Number64::SmallNumber);

	/**
	 * Get a normalized copy of this quaternion.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param Tolerance Minimum squared length of quaternion for normalization.
	 */
	FORCEINLINE FDeterministicQuat64 GetNormalized(FDeterministicNumber64 Tolerance = DeterministicFloat::Constants::Number64::SmallNumber) const;

	// Return true if this quaternion is normalized
	FORCEINLINE bool IsNormalized() const;

	/**
	 * Get the length of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
	FORCEINLINE FDeterministicNumber64 Size() const;

	/**
	 * Get the length squared of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
	FORCEINLINE FDeterministicNumber64 SizeSquared() const;

	/** Get the angle in radians of this quaternion */
	FORCEINLINE FDeterministicNumber64 GetAngle() const;

	/**
	 * get the axis and angle of rotation of this quaternion
	 *
	 * @param Axis{out] Normalized rotation axis of the quaternion
	 * @param Angle{out] Angle of the quaternion in radians
	 * @warning : Requires this quaternion to be normalized.
	 */
	FORCEINLINE void ToAxisAndAngle(FDeterministicVector64& Axis, FDeterministicNumber64& Angle) const;

	/**
	 * Get the rotation vector corresponding to this quaternion.
	 * The direction of the vector represents the rotation axis,
	 * and the magnitude the angle in radians.
	 * @warning : Requires this quaternion to be normalized.
	 */
	FORCEINLINE FDeterministicVector64 ToRotationVector() const;

	/**
	 * Constructs a quaternion corresponding to the rotation vector.
	 * The direction of the vector represents the rotation axis,
	 * and the magnitude the angle in radians.
	 */
	FORCEINLINE static FDeterministicQuat64 MakeFromRotationVector(const FDeterministicVector64& RotationVector);

	/**
	 * @return inverse of this quaternion
	 * @warning : Requires this quaternion to be normalized.
	 */
	FORCEINLINE FDeterministicQuat64 Inverse() const;

	/**
	 * Enforce that the delta between this Quaternion and another one represents
	 * the shortest possible rotation angle
	 */
	FORCEINLINE void EnforceShortestArcWith(const FDeterministicQuat64& OtherQuat);

	/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
	FORCEINLINE FDeterministicVector64 GetAxisX() const;

	/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
	FORCEINLINE FDeterministicVector64 GetAxisY() const;

	/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
	FORCEINLINE FDeterministicVector64 GetAxisZ() const;

	/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
	FORCEINLINE FDeterministicVector64 GetForwardVector() const;

	/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
	FORCEINLINE FDeterministicVector64 GetRightVector() const;

	/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
	FORCEINLINE FDeterministicVector64 GetUpVector() const;

	/** Convert a rotation into a unit vector facing in its direction. Equivalent to GetForwardVector(). */
	FORCEINLINE FDeterministicVector64 Vector() const;

	/** Get the TRotator<T> representation of this Quaternion. */
	FDeterministicRotator64 Rotator() const;

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	FString ToString() const;

	/**
	 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	 */
	static FORCEINLINE FDeterministicQuat64 FindBetween(const FDeterministicVector64& Vector1, const FDeterministicVector64& Vector2);

	/**
	 * Generates the 'smallest' (geodesic) rotation between two normals (assumed to be unit length).
	 */
	static FDeterministicQuat64 FindBetweenNormals(const FDeterministicVector64& Normal1, const FDeterministicVector64& Normal2);

	/**
	 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	 */
	static FDeterministicQuat64 FindBetweenVectors(const FDeterministicVector64& Vector1, const FDeterministicVector64& Vector2);

	/**
	 * Rotate a vector by this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation
	 */
	FDeterministicVector64 RotateVector(FDeterministicVector64 V) const;

	/**
	 * Rotate a vector by the inverse of this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation by the inverse of this quaternion.
	 */
	FDeterministicVector64 UnrotateVector(FDeterministicVector64 V) const;

	/**
	 * @return quaternion with W=0 and V=theta*v.
	 */
	FDeterministicQuat64 Log() const;

	/**
	 * @note Exp should really only be used after Log.
	 * Assumes a quaternion with W=0 and V=theta*v (where |v| = 1).
	 * Exp(q) = (sin(theta)*v, cos(theta))
	 */
	FDeterministicQuat64 Exp() const;

	/**
	 * Fast Linear Quaternion Interpolation.
	 * Result is NOT normalized.
	 */
	static FORCEINLINE FDeterministicQuat64 FastLerp(const FDeterministicQuat64& A, const FDeterministicQuat64& B, const FDeterministicNumber64 Alpha);

	FORCEINLINE operator FQuat() const;
};

// Global operator for (Number32 * Quat)
FORCEINLINE FDeterministicQuat64 operator*(const FDeterministicNumber32 Scale, const FDeterministicQuat64& Q)
{
	return Q.operator*(Scale);
}

// Global operator for (number64 * Quat)
FORCEINLINE FDeterministicQuat64 operator*(const FDeterministicNumber64 Scale, const FDeterministicQuat64& Q)
{
	return Q.operator*(Scale);
}
