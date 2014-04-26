/*
 * @file group.cpp 
 * @brief face group of 3d surface mesh
 * @author Fei Zhu
 * 
 * This file is part of Physika, a versatile physics simulation library.
 * Copyright (C) 2013 Physika Group.
 *
 * This Source Code Form is subject to the terms of the GNU General Public License v2.0. 
 * If a copy of the GPL was not distributed with this file, you can obtain one at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 *
 */

#include <cstddef> 
#include <algorithm>
#include "Physika_Core/Utilities/physika_assert.h"
#include "Physika_Geometry/Surface_Mesh/group.h"

namespace Physika{

namespace SurfaceMeshInternal{

template <typename Scalar>
Group<Scalar>::Group()
    :name_("NONAME"),material_index_(0)
{
}

template <typename Scalar>
Group<Scalar>::~Group()
{
}

template <typename Scalar>
Group<Scalar>::Group(const string &name)
    :name_(name),material_index_(0)
{
}

template <typename Scalar>
Group<Scalar>::Group(const string &name, const vector<Face<Scalar> > &faces)
    :name_(name),material_index_(0)
{
    for(int i = 0; i < faces.size(); ++i)
	addFace(faces[i]);
}

template <typename Scalar>
Group<Scalar>::Group(const string &name, unsigned int material_index)
    :name_(name),material_index_(material_index)
{
}

template <typename Scalar>
Group<Scalar>::Group(const string &name, unsigned int material_index, const vector<Face<Scalar> > &faces)
    :name_(name),material_index_(material_index)
{
    for(int i = 0; i < faces.size(); ++i)
	addFace(faces[i]);
}

template <typename Scalar>
unsigned int Group<Scalar>::numFaces() const
{
    return faces_.size();
}

template <typename Scalar>
const string& Group<Scalar>::name() const
{
    return name_;
}

template <typename Scalar>
void Group<Scalar>::setName(const string &name)
{
    name_ = name;
}

template <typename Scalar>
const Face<Scalar>& Group<Scalar>::face(unsigned int face_idx) const
{
    PHYSIKA_ASSERT(face_idx>=0);
    PHYSIKA_ASSERT(face_idx<faces_.size());
    return faces_[face_idx];
}

template <typename Scalar>
Face<Scalar>& Group<Scalar>::face(unsigned int face_idx)
{
    PHYSIKA_ASSERT(face_idx>=0);
    PHYSIKA_ASSERT(face_idx<faces_.size());
    return faces_[face_idx];
}

template <typename Scalar>
const Face<Scalar>* Group<Scalar>::facePtr(unsigned int face_idx) const
{
    PHYSIKA_ASSERT(face_idx>=0);
    PHYSIKA_ASSERT(face_idx<faces_.size());
    return &(faces_[face_idx]);
}

template <typename Scalar>
Face<Scalar>* Group<Scalar>::facePtr(unsigned int face_idx)
{
    PHYSIKA_ASSERT(face_idx>=0);
    PHYSIKA_ASSERT(face_idx<faces_.size());
    return &(faces_[face_idx]);
}

template <typename Scalar>
unsigned int Group<Scalar>::materialIndex() const
{
    return material_index_;
}

template <typename Scalar>
void Group<Scalar>::setMaterialIndex(unsigned int material_index)
{
    material_index_ = material_index;
}

template <typename Scalar>
void Group<Scalar>::addFace(const Face<Scalar> &face)
{
    faces_.push_back(face);
}

template <typename Scalar>
void Group<Scalar>::removeFace(unsigned int face_idx)
{
    typename vector<Face<Scalar> >::iterator iter = faces_.begin() + face_idx;
    if(iter != faces_.end())
	faces_.erase(iter);
}

//explicit instantitation
template class Group<float>;
template class Group<double>;

} //end of namespace SurfaceMeshInternal

} //end of namespace Physika


















