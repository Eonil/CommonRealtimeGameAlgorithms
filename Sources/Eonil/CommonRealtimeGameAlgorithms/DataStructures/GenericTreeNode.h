//
//  GenericTreeNode.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 5/3/14.
//
//

#pragma once
#include "../CommonRealtimeGameAlgorithmsCommon.h"
#include "TreeNode.h"
EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DATA_STRUCTURES_TOOLS_BEGIN








template <typename T>	class	GenericTreeNode;




template <typename T>
class
GenericTreeNodeIterator : private TreeNodeIterator<std::is_const<T>::value>
{
	static bool const CONSTNESS	=	std::is_const<T>::value;

	using	BASE	=	TreeNodeIterator<CONSTNESS>;
	using	NODE	=	typename std::conditional<std::is_const<T>::value, T const, T>::type;

public:
	/*!
	 The parameter can be null to mark end iterator.
	 */
	GenericTreeNodeIterator(NODE* o) : BASE(o)
	{
	}
	
	auto
	operator!=(GenericTreeNodeIterator const&o) const -> bool
	{
		return	BASE::operator!=(o);
	}
	auto
	operator*() const -> NODE&
	{
		return	static_cast<NODE&>(BASE::operator*());
	}
	auto
	operator++() -> void
	{
		BASE::operator++();
	}
};

template <typename T>
class
GenericTreeNodeRange : private TreeNodeRange<std::is_const<T>::value>
{
	static bool const CONSTNESS	=	std::is_const<T>::value;
	
	using	BASE	=	TreeNodeRange<CONSTNESS>;
	using	NODE	=	typename std::conditional<std::is_const<T>::value, T const, T>::type;
	using	ITER	=	typename std::conditional<std::is_const<T>::value, GenericTreeNodeIterator<T const>, GenericTreeNodeIterator<T>>::type;
	
public:
	GenericTreeNodeRange() = default;								//!	Creates empty (zero-node) range (list).
	
	operator GenericTreeNodeRange<T const>() const
	{
		return	{&BASE::front(), &BASE::back()};
	}
	
	////
	
	/*!
	 Creates non-empty (one or more nodes) range. Both parameters cannot be null.
	 */
	GenericTreeNodeRange(NODE* first, NODE* last) : BASE(first, last)
	{
	}
	
	auto
	begin() const -> ITER
	{
		return	ITER(&static_cast<NODE&>(*BASE::begin()));
	}
	auto
	end() const -> ITER
	{
		return	ITER(&static_cast<NODE&>(*BASE::end()));
//		return	ITER(&*BASE::end());
//		return	ITER(nullptr);
	}
};

















template <typename T>
class
GenericTreeNode : private TreeNode
{
	friend class	GenericTreeNodeIterator<T>;
	friend class	GenericTreeNodeRange<T>;
	
	using			NODE	=	T;
	
public:
	~GenericTreeNode() noexcept
	{
		static_assert(std::is_base_of<GenericTreeNode<T>, T>::value, "The type `T` must be a subclass of `GenericTreeNode<T>`.");
	}
	
	auto
	prior() const -> NODE const*
	{
		return	static_cast<NODE const*>(TreeNode::prior());
	}
	auto
	prior() -> NODE*
	{
		return	static_cast<NODE*>(TreeNode::prior());
	}
	auto
	setPrior(NODE* o) -> void
	{
		TreeNode::setPrior(o);
	}
	auto
	unsetPrior() -> void
	{
		TreeNode::unsetParent();
	}
	
	auto
	next() const -> NODE const*
	{
		return	static_cast<NODE const*>(TreeNode::next());
	}
	auto
	next() -> NODE*
	{
		return	static_cast<NODE*>(TreeNode::next());
	}
	auto
	setNext(NODE* o) -> void
	{
		TreeNode::setNext(o);
	}
	auto
	unsetNext() -> void
	{
		TreeNode::unsetNext();
	}
	
	auto
	parent() const -> NODE const*
	{
		return	static_cast<NODE const*>(TreeNode::parent());
	}
	auto
	parent() -> NODE*
	{
		return	static_cast<NODE*>(TreeNode::parent());
	}
	auto
	setParent(NODE* o) -> void
	{
		TreeNode::setParent(o);
	}
	auto
	unsetParent() -> void
	{
		TreeNode::unsetParent();
	}
	
	auto
	children() const -> GenericTreeNodeRange<T const>
	{
		auto	cr	=	TreeNode::children();
		return	{static_cast<NODE const*>(&cr.front()), static_cast<NODE const*>(&cr.back())};
	}
	auto
	children() -> GenericTreeNodeRange<T>
	{
		auto	cr	=	TreeNode::children();
		return	{static_cast<NODE*>(&cr.front()), static_cast<NODE*>(&cr.back())};
	}
	auto
	setChildren(GenericTreeNodeRange<T> o) -> void
	{
		TreeNode::setChildren(o);
	}
	auto
	unsetChildren() -> void
	{
		TreeNode::unsetChildren();
	}
};
























EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DATA_STRUCTURES_TOOLS_END