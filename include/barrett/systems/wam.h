/** Defines barrett::Wam.
 *
 * @file barrett/wam.h
 * @date Sep 25, 2009
 * @author Dan Cody
 */

/* Copyright 2009 Barrett Technology <support@barrett.com> */

/* This file is part of libbarrett.
 *
 * This version of libbarrett is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This version of libbarrett is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this version of libbarrett.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Further, non-binding information about licensing is available at:
 * <http://wiki.barrett.com/libbarrett/wiki/LicenseNotes>
 */


#ifndef BARRETT_SYSTEMS_WAM_H_
#define BARRETT_SYSTEMS_WAM_H_


#include <vector>

#include <libconfig.h++>

#include <barrett/detail/ca_macro.h>
#include <barrett/units.h>
#include <barrett/systems/abstract/system.h>

#include <barrett/systems/low_level_wam_wrapper.h>
#include <barrett/systems/first_order_filter.h>
#include <barrett/systems/converter.h>
#include <barrett/systems/summer.h>
#include <barrett/systems/gain.h>

#include <barrett/systems/kinematics_base.h>
#include <barrett/systems/gravity_compensator.h>
#include <barrett/systems/tool_position.h>
#include <barrett/systems/tool_orientation.h>

#include <barrett/systems/pid_controller.h>
#include <barrett/systems/tool_orientation_controller.h>
#include <barrett/systems/tool_force_to_joint_torques.h>


namespace barrett {
namespace systems {


template<size_t DOF>
class Wam {
	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);

public:

	// these need to be before the IO references
	LowLevelWamWrapper<DOF> llww;
	KinematicsBase<DOF> kinematicsBase;
	GravityCompensator<DOF> gravity;
	FirstOrderFilter<jv_type> jvFilter;
	ToolPosition<DOF> toolPosition;
	ToolOrientation<DOF> toolOrientation;

	Converter<jt_type> supervisoryController;
	Gain<jt_type, double> jtPassthrough;
	PIDController<jp_type, jt_type> jpController;
	PIDController<jv_type, jt_type> jvController1;
	FirstOrderFilter<jt_type> jvController2;
	PIDController<cp_type, cf_type> tpController;
	ToolForceToJointTorques<DOF> tf2jt;
	ToolOrientationController<DOF> toController;

	Summer<jt_type, 3> jtSum;
	enum {JT_INPUT = 0, GRAVITY_INPUT, SC_INPUT};


// IO
public:		System::Input<jt_type>& input;
public:		System::Output<jp_type>& jpOutput;
public:		System::Output<jv_type>& jvOutput;


public:
	// genericPucks must be ordered by joint and must break into torque groups as arranged
	Wam(const std::vector<Puck*>& genericPucks, Puck* safetyPuck,
			const libconfig::Setting& setting,
			std::vector<int> torqueGroupIds = std::vector<int>());
	~Wam();

	template<typename T>
	void trackReferenceSignal(System::Output<T>& referenceSignal);  //NOLINT: non-const reference for syntax

	const jp_type& getHomePosition() const;
	jt_type getJointTorques() const;
	jp_type getJointPositions() const;
	jv_type getJointVelocities() const;
	cp_type getToolPosition() const;
	Eigen::Quaterniond getToolOrientation() const;


	void gravityCompensate(bool compensate = true);
	void moveHome(bool blocking = true, double velocity = 0.5, double acceleration = 0.5);
	void moveTo(const jp_type& destination, bool blocking = true, double velocity = 0.5, double acceleration = 0.5);
	template<typename T> void moveTo(const T& currentPos, const typename T::unitless_type& currentVel, const T& destination, bool blocking, double velocity, double acceleration);
	bool moveIsDone() const;
	void idle();

protected:
	template<typename T> void moveToThread(const T& currentPos, const typename T::unitless_type& currentVel, const T& destination, double velocity, double acceleration);

	bool doneMoving;

private:
	DISALLOW_COPY_AND_ASSIGN(Wam);
};


}
}


// include template definitions
#include <barrett/systems/detail/wam-inl.h>


#endif /* BARRETT_SYSTEMS_WAM_H_ */