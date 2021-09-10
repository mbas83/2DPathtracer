#pragma once

#include <glm/glm.hpp>

class Material
{
public:
	Material(glm::vec3 _reflection_color) :reflection_color(_reflection_color) {}
	virtual ~Material() = default;

	/// Compute the factor for light reflected from incident into excident direction.
	///
	/// This method is called by for direct illumination computations.
	/// \param [in] _incident Normalized direction vector pointing away from the
	///		surface to the observer.
	/// \param [in] _excident Normalized direction vector pointing away from the
	///		surface to the light source.
	/// \param [in] _normal Normalized direction vector perpendicular to the surface.
	/// \return A factor for reflected light for "each frequency" using RGB model.
	virtual glm::vec3 operator()(const glm::vec2& _incident, const glm::vec2& _excident, const glm::vec2& _normal) const = 0;

	/// Create a random reflection/refraction direction.
	///
	/// This method is called at hit points for the material of the according object.
	/// The sampling should follow the importance sampling for the current material.
	/// I.e. directions must be produced proportional to the reflected distribution.
	/// \param [in] _incident Normalized direction vector pointing away from the
	///		surface to the observer.
	/// \param [out] _probability Return sampling probability for Monte Carlo weight.
	///		Since the PDF is continuous it can have values > 1. Its area is 1!
	virtual glm::vec2 sample_dir(const glm::vec2& _incident, const glm::vec2& _normal, float& _probability) = 0;


	/// \param [in] _normal Normalized direction vector perpendicular to the surface.
	/// \return the intensity for materials that emit light
	virtual glm::vec3 get_self_emitting_value(const glm::vec2& _normal) const { return glm::vec3(0.0f); }

protected:
	glm::vec3 reflection_color;
};