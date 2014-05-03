//
//  TreeNode.h
//  CommonRealtimeGameAlgorithms
//
//  Created by Hoon H. on 5/3/14.
//
//

#pragma once
#include "../CommonRealtimeGameAlgorithmsCommon.h"
EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DATA_STRUCTURES_TOOLS_BEGIN















template <bool const CONSTNESS>		class	TreeNodeIterator;
template <bool const CONSTNESS>		class	TreeNodeRange;



/*!
 Organizes and manages tree structure.
 
 @classdesign
 A tree-node is all about identity. Copy or move semantics cannot be defined.
 As a result, you cannot create this in a container which doesn't support `emplace`.
 */
class
TreeNode
{
	using	NODE	=	TreeNode;
	
public:

public:
	TreeNode() = default;
	TreeNode(TreeNode const&) = delete;
	TreeNode(TreeNode&&) = delete;
	~TreeNode() = default;
	
	auto	operator=(TreeNode const&) -> TreeNode& = delete;
	auto	operator=(TreeNode&&) -> TreeNode& = delete;
	
	////
	
	auto	prior() const -> TreeNode const*;
	auto	prior() -> TreeNode*;
	auto	setPrior(TreeNode*) -> void;
	auto	unsetPrior() -> void;
	
	auto	next() const -> TreeNode const*;
	auto	next() -> TreeNode*;
	auto	setNext(TreeNode*) -> void;
	auto	unsetNext() -> void;
	
	auto	parent() const -> TreeNode const*;
	auto	parent() -> TreeNode*;
	auto	setParent(TreeNode*) -> void;					//!	Take care that this method is valid only for single child node. (supplied node should have no linked prior/next node) Because it's impossible to set parent/child/prior/next at once.
	auto	unsetParent() -> void;
	
	auto	children() const -> TreeNodeRange<true>;
	auto	children() -> TreeNodeRange<false>;
	auto	setChildren(TreeNodeRange<false>) -> void;
	auto	unsetChildren() -> void;
	
private:
	NODE*	_prior	=	nullptr;
	NODE*	_next	=	nullptr;
	NODE*	_parent	=	nullptr;
	NODE*	_fchild	=	nullptr;
	NODE*	_lchild	=	nullptr;
	
	auto	_contains(NODE const*) const -> bool;			//!	Checks whether the node is exists in current subtree. (including this node).
	auto	_first() const -> NODE const*;
	auto	_last() const -> NODE const*;
	
	////
	
private:
	friend class	TreeNodeIterator<true>;
	friend class	TreeNodeIterator<false>;
	friend class	TreeNodeRange<true>;
	friend class	TreeNodeRange<false>;
	
	/*!
	 Circularity can be disabled optionally because it costs too much.
	 */
	static bool const	USE_CIRCULAR_LINKING_CHECKINGS	=	USE_HEAVY_EXCEPTION_CHECKINGS;
	
	auto	_halt_if_this_is_null() const -> void;
	auto	_halt_if_children_linking_is_not_consistent() const -> void;
	auto	_error_if_supplied_children_linking_is_not_consistent(TreeNodeRange<true>) const -> void;
	auto	_error_if_supplied_node_is_null(NODE const*) const -> void;
	auto	_error_if_supplied_node_would_make_circular_linkage(NODE const*) const -> void;
	
};


template <bool const CONSTNESS>
class
TreeNodeIterator
{
	using	TYPE	=	typename std::conditional<CONSTNESS, TreeNode const, TreeNode>::type;
	
public:
//	TreeNodeIterator(std::nullptr_t);		//!	Creates the `end` iterator.
//	TreeNodeIterator(TYPE*);				//!	Creates non-`end` iterator.
	TreeNodeIterator(TYPE*);				//!	The parameter can be null to mark end iterator.
	
	auto	operator!=(TreeNodeIterator const&) const -> bool;
	auto	operator*() const -> TYPE&;
//	auto	operator->() -> TYPE*;
//	auto	operator--() -> void;			//!	`TreeNode` is back-navigatable, but it needs extra informations, so disabled on iterator.
	auto	operator++() -> void;
	
private:
	TYPE*	_ptr	=	nullptr;
};

template <bool const CONSTNESS>
class
TreeNodeRange
{
	using	NODE	=	typename std::conditional<CONSTNESS, TreeNode const, TreeNode>::type;
	
public:
	TreeNodeRange() = default;								//!	Creates empty (zero-node) range (list).
//	TreeNodeRange(std::nullptr_t) = default;			//!	Creates empty (zero-node) range (list).
	
	operator TreeNodeRange<true>() const;
	
	////
	
	TreeNodeRange(NODE* first, NODE* last);					//!	Creates non-empty (one or more nodes) range. Both parameters cannot be null.
	
	auto	front() const -> NODE&;
	auto	back() const -> NODE&;
	
	auto	begin() const -> TreeNodeIterator<CONSTNESS>;
	auto	end() const -> TreeNodeIterator<CONSTNESS>;
	
protected:
//private:
	friend class		TreeNode;
	NODE*	_first	=	nullptr;
	NODE*	_last	=	nullptr;
};












































EONIL_COMMON_REALTIME_GAME_ALGORITHMS_DATA_STRUCTURES_TOOLS_END