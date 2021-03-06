// Copyright (c) 2015-2017 Vector 35 LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#ifdef BINARYNINJACORE_LIBRARY
#include "lowlevelilfunction.h"
#include "lowlevelilssafunction.h"
#include "mediumlevelilfunction.h"
using namespace BinaryNinjaCore;
#else
#include "binaryninjaapi.h"
#include "lowlevelilinstruction.h"
#include "mediumlevelilinstruction.h"
using namespace BinaryNinja;
#endif

using namespace std;


unordered_map<LowLevelILOperandUsage, LowLevelILOperandType>
	LowLevelILInstructionBase::operandTypeForUsage = {
		{SourceExprLowLevelOperandUsage, ExprLowLevelOperand},
		{SourceRegisterLowLevelOperandUsage, RegisterLowLevelOperand},
		{SourceFlagLowLevelOperandUsage, FlagLowLevelOperand},
		{SourceSSARegisterLowLevelOperandUsage, SSARegisterLowLevelOperand},
		{SourceSSAFlagLowLevelOperandUsage, SSAFlagLowLevelOperand},
		{DestExprLowLevelOperandUsage, ExprLowLevelOperand},
		{DestRegisterLowLevelOperandUsage, RegisterLowLevelOperand},
		{DestFlagLowLevelOperandUsage, FlagLowLevelOperand},
		{DestSSARegisterLowLevelOperandUsage, SSARegisterLowLevelOperand},
		{DestSSAFlagLowLevelOperandUsage, SSAFlagLowLevelOperand},
		{PartialRegisterLowLevelOperandUsage, RegisterLowLevelOperand},
		{StackSSARegisterLowLevelOperandUsage, SSARegisterLowLevelOperand},
		{StackMemoryVersionLowLevelOperandUsage, IndexLowLevelOperand},
		{LeftExprLowLevelOperandUsage, ExprLowLevelOperand},
		{RightExprLowLevelOperandUsage, ExprLowLevelOperand},
		{CarryExprLowLevelOperandUsage, ExprLowLevelOperand},
		{HighExprLowLevelOperandUsage, ExprLowLevelOperand},
		{LowExprLowLevelOperandUsage, ExprLowLevelOperand},
		{ConditionExprLowLevelOperandUsage, ExprLowLevelOperand},
		{HighRegisterLowLevelOperandUsage, RegisterLowLevelOperand},
		{HighSSARegisterLowLevelOperandUsage, SSARegisterLowLevelOperand},
		{LowRegisterLowLevelOperandUsage, RegisterLowLevelOperand},
		{LowSSARegisterLowLevelOperandUsage, SSARegisterLowLevelOperand},
		{ConstantLowLevelOperandUsage, IntegerLowLevelOperand},
		{VectorLowLevelOperandUsage, IntegerLowLevelOperand},
		{StackAdjustmentLowLevelOperandUsage, IntegerLowLevelOperand},
		{TargetLowLevelOperandUsage, IndexLowLevelOperand},
		{TrueTargetLowLevelOperandUsage, IndexLowLevelOperand},
		{FalseTargetLowLevelOperandUsage, IndexLowLevelOperand},
		{BitIndexLowLevelOperandUsage, IndexLowLevelOperand},
		{SourceMemoryVersionLowLevelOperandUsage, IndexLowLevelOperand},
		{DestMemoryVersionLowLevelOperandUsage, IndexLowLevelOperand},
		{FlagConditionLowLevelOperandUsage, FlagConditionLowLevelOperand},
		{OutputSSARegistersLowLevelOperandUsage, SSARegisterListLowLevelOperand},
		{OutputMemoryVersionLowLevelOperandUsage, IndexLowLevelOperand},
		{ParameterSSARegistersLowLevelOperandUsage, SSARegisterListLowLevelOperand},
		{SourceSSARegistersLowLevelOperandUsage, SSARegisterListLowLevelOperand},
		{SourceSSAFlagsLowLevelOperandUsage, SSAFlagListLowLevelOperand},
		{SourceMemoryVersionsLowLevelOperandUsage, IndexListLowLevelOperand},
		{TargetListLowLevelOperandUsage, IndexListLowLevelOperand}
	};


unordered_map<BNLowLevelILOperation, vector<LowLevelILOperandUsage>>
	LowLevelILInstructionBase::operationOperandUsage = {
		{LLIL_NOP, {}},
		{LLIL_POP, {}},
		{LLIL_NORET, {}},
		{LLIL_SYSCALL, {}},
		{LLIL_BP, {}},
		{LLIL_UNDEF, {}},
		{LLIL_UNIMPL, {}},
		{LLIL_SET_REG, {DestRegisterLowLevelOperandUsage, SourceExprLowLevelOperandUsage}},
		{LLIL_SET_REG_SPLIT, {HighRegisterLowLevelOperandUsage, LowRegisterLowLevelOperandUsage,
			SourceExprLowLevelOperandUsage}},
		{LLIL_SET_REG_SSA, {DestSSARegisterLowLevelOperandUsage, SourceExprLowLevelOperandUsage}},
		{LLIL_SET_REG_SSA_PARTIAL, {DestSSARegisterLowLevelOperandUsage, PartialRegisterLowLevelOperandUsage,
			SourceExprLowLevelOperandUsage}},
		{LLIL_SET_REG_SPLIT_SSA, {HighSSARegisterLowLevelOperandUsage,
			LowSSARegisterLowLevelOperandUsage, SourceExprLowLevelOperandUsage}},
		{LLIL_SET_FLAG, {DestFlagLowLevelOperandUsage, SourceExprLowLevelOperandUsage}},
		{LLIL_SET_FLAG_SSA, {DestSSAFlagLowLevelOperandUsage, SourceExprLowLevelOperandUsage}},
		{LLIL_LOAD, {SourceExprLowLevelOperandUsage}},
		{LLIL_LOAD_SSA, {SourceExprLowLevelOperandUsage, SourceMemoryVersionLowLevelOperandUsage}},
		{LLIL_STORE, {DestExprLowLevelOperandUsage, SourceExprLowLevelOperandUsage}},
		{LLIL_STORE_SSA, {DestExprLowLevelOperandUsage, DestMemoryVersionLowLevelOperandUsage,
			SourceMemoryVersionLowLevelOperandUsage, SourceExprLowLevelOperandUsage}},
		{LLIL_REG, {SourceRegisterLowLevelOperandUsage}},
		{LLIL_REG_SSA, {SourceSSARegisterLowLevelOperandUsage}},
		{LLIL_REG_SSA_PARTIAL, {SourceSSARegisterLowLevelOperandUsage, PartialRegisterLowLevelOperandUsage}},
		{LLIL_FLAG, {SourceFlagLowLevelOperandUsage}},
		{LLIL_FLAG_BIT, {SourceFlagLowLevelOperandUsage, BitIndexLowLevelOperandUsage}},
		{LLIL_FLAG_SSA, {SourceSSAFlagLowLevelOperandUsage}},
		{LLIL_FLAG_BIT_SSA, {SourceSSAFlagLowLevelOperandUsage, BitIndexLowLevelOperandUsage}},
		{LLIL_JUMP, {DestExprLowLevelOperandUsage}},
		{LLIL_JUMP_TO, {DestExprLowLevelOperandUsage, TargetListLowLevelOperandUsage}},
		{LLIL_CALL, {DestExprLowLevelOperandUsage}},
		{LLIL_CALL_STACK_ADJUST, {DestExprLowLevelOperandUsage, StackAdjustmentLowLevelOperandUsage}},
		{LLIL_RET, {DestExprLowLevelOperandUsage}},
		{LLIL_IF, {ConditionExprLowLevelOperandUsage, TrueTargetLowLevelOperandUsage,
			FalseTargetLowLevelOperandUsage}},
		{LLIL_GOTO, {TargetLowLevelOperandUsage}},
		{LLIL_FLAG_COND, {FlagConditionLowLevelOperandUsage}},
		{LLIL_TRAP, {VectorLowLevelOperandUsage}},
		{LLIL_CALL_SSA, {OutputSSARegistersLowLevelOperandUsage, OutputMemoryVersionLowLevelOperandUsage,
			DestExprLowLevelOperandUsage, StackSSARegisterLowLevelOperandUsage,
			StackMemoryVersionLowLevelOperandUsage, ParameterSSARegistersLowLevelOperandUsage}},
		{LLIL_SYSCALL_SSA, {OutputSSARegistersLowLevelOperandUsage, OutputMemoryVersionLowLevelOperandUsage,
			StackSSARegisterLowLevelOperandUsage, StackMemoryVersionLowLevelOperandUsage,
			ParameterSSARegistersLowLevelOperandUsage}},
		{LLIL_REG_PHI, {DestSSARegisterLowLevelOperandUsage, SourceSSARegistersLowLevelOperandUsage}},
		{LLIL_FLAG_PHI, {DestSSAFlagLowLevelOperandUsage, SourceSSAFlagsLowLevelOperandUsage}},
		{LLIL_MEM_PHI, {DestMemoryVersionLowLevelOperandUsage, SourceMemoryVersionsLowLevelOperandUsage}},
		{LLIL_CONST, {ConstantLowLevelOperandUsage}},
		{LLIL_CONST_PTR, {ConstantLowLevelOperandUsage}},
		{LLIL_ADD, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_SUB, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_AND, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_OR, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_XOR, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_LSL, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_LSR, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_ASR, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_ROL, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_ROR, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_MUL, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_MULU_DP, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_MULS_DP, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_DIVU, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_DIVS, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_MODU, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_MODS, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_CMP_E, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_CMP_NE, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_CMP_SLT, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_CMP_ULT, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_CMP_SLE, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_CMP_ULE, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_CMP_SGE, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_CMP_UGE, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_CMP_SGT, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_CMP_UGT, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_TEST_BIT, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_ADD_OVERFLOW, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_ADC, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage, CarryExprLowLevelOperandUsage}},
		{LLIL_SBB, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage, CarryExprLowLevelOperandUsage}},
		{LLIL_RLC, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage, CarryExprLowLevelOperandUsage}},
		{LLIL_RRC, {LeftExprLowLevelOperandUsage, RightExprLowLevelOperandUsage, CarryExprLowLevelOperandUsage}},
		{LLIL_DIVU_DP, {HighExprLowLevelOperandUsage, LowExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_DIVS_DP, {HighExprLowLevelOperandUsage, LowExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_MODU_DP, {HighExprLowLevelOperandUsage, LowExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_MODS_DP, {HighExprLowLevelOperandUsage, LowExprLowLevelOperandUsage, RightExprLowLevelOperandUsage}},
		{LLIL_PUSH, {SourceExprLowLevelOperandUsage}},
		{LLIL_NEG, {SourceExprLowLevelOperandUsage}},
		{LLIL_NOT, {SourceExprLowLevelOperandUsage}},
		{LLIL_SX, {SourceExprLowLevelOperandUsage}},
		{LLIL_ZX, {SourceExprLowLevelOperandUsage}},
		{LLIL_LOW_PART, {SourceExprLowLevelOperandUsage}},
		{LLIL_BOOL_TO_INT, {SourceExprLowLevelOperandUsage}},
		{LLIL_UNIMPL_MEM, {SourceExprLowLevelOperandUsage}}
	};


static unordered_map<BNLowLevelILOperation, unordered_map<LowLevelILOperandUsage, size_t>> GetOperandIndexForOperandUsages()
{
	unordered_map<BNLowLevelILOperation, unordered_map<LowLevelILOperandUsage, size_t>> result;
	result.reserve(LowLevelILInstructionBase::operationOperandUsage.size());
	for (auto& operation : LowLevelILInstructionBase::operationOperandUsage)
	{
		result[operation.first] = unordered_map<LowLevelILOperandUsage, size_t>();

		size_t operand = 0;
		result[operation.first].reserve(operation.second.size());
		for (auto usage : operation.second)
		{
			result[operation.first][usage] = operand;
			switch (usage)
			{
			case HighSSARegisterLowLevelOperandUsage:
			case LowSSARegisterLowLevelOperandUsage:
				// Represented as subexpression, so only takes one slot even though it is an SSA register
				operand++;
				break;
			case ParameterSSARegistersLowLevelOperandUsage:
				// Represented as subexpression, so only takes one slot even though it is a list
				operand++;
				break;
			case OutputSSARegistersLowLevelOperandUsage:
				// OutputMemoryVersionLowLevelOperandUsage follows at same operand
				break;
			case StackSSARegisterLowLevelOperandUsage:
				// StackMemoryVersionLowLevelOperandUsage follows at same operand
				break;
			default:
				switch (LowLevelILInstructionBase::operandTypeForUsage[usage])
				{
				case SSARegisterLowLevelOperand:
				case SSAFlagLowLevelOperand:
				case IndexListLowLevelOperand:
				case SSARegisterListLowLevelOperand:
				case SSAFlagListLowLevelOperand:
					// SSA registers/flags and lists take two operand slots
					operand += 2;
					break;
				default:
					operand++;
					break;
				}
				break;
			}
		}
	}
	return result;
}


unordered_map<BNLowLevelILOperation, unordered_map<LowLevelILOperandUsage, size_t>>
	LowLevelILInstructionBase::operationOperandIndex = GetOperandIndexForOperandUsages();


SSARegister::SSARegister(): reg(BN_INVALID_REGISTER), version(0)
{
}


SSARegister::SSARegister(const uint32_t r, size_t i): reg(r), version(i)
{
}


SSARegister::SSARegister(const SSARegister& v): reg(v.reg), version(v.version)
{
}


SSARegister& SSARegister::operator=(const SSARegister& v)
{
	reg = v.reg;
	version = v.version;
	return *this;
}


bool SSARegister::operator==(const SSARegister& v) const
{
	if (reg != v.reg)
		return false;
	return version == v.version;
}


bool SSARegister::operator!=(const SSARegister& v) const
{
	return !((*this) == v);
}


bool SSARegister::operator<(const SSARegister& v) const
{
	if (reg < v.reg)
		return true;
	if (v.reg < reg)
		return false;
	return version < v.version;
}


SSAFlag::SSAFlag(): flag(BN_INVALID_REGISTER), version(0)
{
}


SSAFlag::SSAFlag(const uint32_t f, size_t i): flag(f), version(i)
{
}


SSAFlag::SSAFlag(const SSAFlag& v): flag(v.flag), version(v.version)
{
}


SSAFlag& SSAFlag::operator=(const SSAFlag& v)
{
	flag = v.flag;
	version = v.version;
	return *this;
}


bool SSAFlag::operator==(const SSAFlag& v) const
{
	if (flag != v.flag)
		return false;
	return version == v.version;
}


bool SSAFlag::operator!=(const SSAFlag& v) const
{
	return !((*this) == v);
}


bool SSAFlag::operator<(const SSAFlag& v) const
{
	if (flag < v.flag)
		return true;
	if (v.flag < flag)
		return false;
	return version < v.version;
}


bool LowLevelILIntegerList::ListIterator::operator==(const ListIterator& a) const
{
	return count == a.count;
}


bool LowLevelILIntegerList::ListIterator::operator!=(const ListIterator& a) const
{
	return count != a.count;
}


bool LowLevelILIntegerList::ListIterator::operator<(const ListIterator& a) const
{
	return count > a.count;
}


LowLevelILIntegerList::ListIterator& LowLevelILIntegerList::ListIterator::operator++()
{
	count--;
	if (count == 0)
		return *this;

	operand++;
	if (operand >= 3)
	{
		operand = 0;
#ifdef BINARYNINJACORE_LIBRARY
		instr = &function->GetRawExpr((size_t)instr->operands[3]);
#else
		instr = function->GetRawExpr((size_t)instr.operands[3]);
#endif
	}
	return *this;
}


uint64_t LowLevelILIntegerList::ListIterator::operator*()
{
#ifdef BINARYNINJACORE_LIBRARY
	return instr->operands[operand];
#else
	return instr.operands[operand];
#endif
}


LowLevelILIntegerList::LowLevelILIntegerList(LowLevelILFunction* func,
const BNLowLevelILInstruction& instr, size_t count)
{
	m_start.function = func;
#ifdef BINARYNINJACORE_LIBRARY
	m_start.instr = &instr;
#else
	m_start.instr = instr;
#endif
	m_start.operand = 0;
	m_start.count = count;
}


LowLevelILIntegerList::const_iterator LowLevelILIntegerList::begin() const
{
	return m_start;
}


LowLevelILIntegerList::const_iterator LowLevelILIntegerList::end() const
{
	const_iterator result;
	result.function = m_start.function;
	result.operand = 0;
	result.count = 0;
	return result;
}


size_t LowLevelILIntegerList::size() const
{
	return m_start.count;
}


uint64_t LowLevelILIntegerList::operator[](size_t i) const
{
	if (i >= size())
		throw LowLevelILInstructionAccessException();
	auto iter = begin();
	for (size_t j = 0; j < i; j++)
		++iter;
	return *iter;
}


LowLevelILIntegerList::operator vector<uint64_t>() const
{
	vector<uint64_t> result;
	for (auto i : *this)
		result.push_back(i);
	return result;
}


size_t LowLevelILIndexList::ListIterator::operator*()
{
	return (size_t)*pos;
}


LowLevelILIndexList::LowLevelILIndexList(LowLevelILFunction* func,
	const BNLowLevelILInstruction& instr, size_t count): m_list(func, instr, count)
{
}


LowLevelILIndexList::const_iterator LowLevelILIndexList::begin() const
{
	const_iterator result;
	result.pos = m_list.begin();
	return result;
}


LowLevelILIndexList::const_iterator LowLevelILIndexList::end() const
{
	const_iterator result;
	result.pos = m_list.end();
	return result;
}


size_t LowLevelILIndexList::size() const
{
	return m_list.size();
}


size_t LowLevelILIndexList::operator[](size_t i) const
{
	if (i >= size())
		throw LowLevelILInstructionAccessException();
	auto iter = begin();
	for (size_t j = 0; j < i; j++)
		++iter;
	return *iter;
}


LowLevelILIndexList::operator vector<size_t>() const
{
	vector<size_t> result;
	for (auto i : *this)
		result.push_back(i);
	return result;
}


const SSARegister LowLevelILSSARegisterList::ListIterator::operator*()
{
	LowLevelILIntegerList::const_iterator cur = pos;
	uint32_t reg = (uint32_t)*cur;
	++cur;
	size_t version = (size_t)*cur;
	return SSARegister(reg, version);
}


LowLevelILSSARegisterList::LowLevelILSSARegisterList(LowLevelILFunction* func,
	const BNLowLevelILInstruction& instr, size_t count): m_list(func, instr, count & (~1))
{
}


LowLevelILSSARegisterList::const_iterator LowLevelILSSARegisterList::begin() const
{
	const_iterator result;
	result.pos = m_list.begin();
	return result;
}


LowLevelILSSARegisterList::const_iterator LowLevelILSSARegisterList::end() const
{
	const_iterator result;
	result.pos = m_list.end();
	return result;
}


size_t LowLevelILSSARegisterList::size() const
{
	return m_list.size() / 2;
}


const SSARegister LowLevelILSSARegisterList::operator[](size_t i) const
{
	if (i >= size())
		throw LowLevelILInstructionAccessException();
	auto iter = begin();
	for (size_t j = 0; j < i; j++)
		++iter;
	return *iter;
}


LowLevelILSSARegisterList::operator vector<SSARegister>() const
{
	vector<SSARegister> result;
	for (auto& i : *this)
		result.push_back(i);
	return result;
}


const SSAFlag LowLevelILSSAFlagList::ListIterator::operator*()
{
	LowLevelILIntegerList::const_iterator cur = pos;
	uint32_t flag = (uint32_t)*cur;
	++cur;
	size_t version = (size_t)*cur;
	return SSAFlag(flag, version);
}


LowLevelILSSAFlagList::LowLevelILSSAFlagList(LowLevelILFunction* func,
	const BNLowLevelILInstruction& instr, size_t count): m_list(func, instr, count & (~1))
{
}


LowLevelILSSAFlagList::const_iterator LowLevelILSSAFlagList::begin() const
{
	const_iterator result;
	result.pos = m_list.begin();
	return result;
}


LowLevelILSSAFlagList::const_iterator LowLevelILSSAFlagList::end() const
{
	const_iterator result;
	result.pos = m_list.end();
	return result;
}


size_t LowLevelILSSAFlagList::size() const
{
	return m_list.size() / 2;
}


const SSAFlag LowLevelILSSAFlagList::operator[](size_t i) const
{
	if (i >= size())
		throw LowLevelILInstructionAccessException();
	auto iter = begin();
	for (size_t j = 0; j < i; j++)
		++iter;
	return *iter;
}


LowLevelILSSAFlagList::operator vector<SSAFlag>() const
{
	vector<SSAFlag> result;
	for (auto& i : *this)
		result.push_back(i);
	return result;
}


LowLevelILOperand::LowLevelILOperand(const LowLevelILInstruction& instr,
	LowLevelILOperandUsage usage, size_t operandIndex):
	m_instr(instr), m_usage(usage), m_operandIndex(operandIndex)
{
	auto i = LowLevelILInstructionBase::operandTypeForUsage.find(m_usage);
	if (i == LowLevelILInstructionBase::operandTypeForUsage.end())
		throw LowLevelILInstructionAccessException();
	m_type = i->second;
}


uint64_t LowLevelILOperand::GetInteger() const
{
	if (m_type != IntegerLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	return m_instr.GetRawOperandAsInteger(m_operandIndex);
}


size_t LowLevelILOperand::GetIndex() const
{
	if (m_type != IndexLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	if (m_usage == OutputMemoryVersionLowLevelOperandUsage)
		return m_instr.GetRawOperandAsExpr(m_operandIndex).GetRawOperandAsIndex(0);
	if (m_usage == StackMemoryVersionLowLevelOperandUsage)
		return m_instr.GetRawOperandAsExpr(m_operandIndex).GetRawOperandAsIndex(2);
	return m_instr.GetRawOperandAsIndex(m_operandIndex);
}


LowLevelILInstruction LowLevelILOperand::GetExpr() const
{
	if (m_type != ExprLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	return m_instr.GetRawOperandAsExpr(m_operandIndex);
}


uint32_t LowLevelILOperand::GetRegister() const
{
	if (m_type != RegisterLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	return m_instr.GetRawOperandAsRegister(m_operandIndex);
}


uint32_t LowLevelILOperand::GetFlag() const
{
	if (m_type != FlagLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	return m_instr.GetRawOperandAsRegister(m_operandIndex);
}


BNLowLevelILFlagCondition LowLevelILOperand::GetFlagCondition() const
{
	if (m_type != FlagConditionLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	return m_instr.GetRawOperandAsFlagCondition(m_operandIndex);
}


SSARegister LowLevelILOperand::GetSSARegister() const
{
	if (m_type != SSARegisterLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	if ((m_usage == HighSSARegisterLowLevelOperandUsage) || (m_usage == LowSSARegisterLowLevelOperandUsage) ||
		(m_usage == StackSSARegisterLowLevelOperandUsage))
		return m_instr.GetRawOperandAsExpr(m_operandIndex).GetRawOperandAsSSARegister(0);
	return m_instr.GetRawOperandAsSSARegister(m_operandIndex);
}


SSAFlag LowLevelILOperand::GetSSAFlag() const
{
	if (m_type != SSAFlagLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	return m_instr.GetRawOperandAsSSAFlag(m_operandIndex);
}


LowLevelILIndexList LowLevelILOperand::GetIndexList() const
{
	if (m_type != IndexListLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	return m_instr.GetRawOperandAsIndexList(m_operandIndex);
}


LowLevelILSSARegisterList LowLevelILOperand::GetSSARegisterList() const
{
	if (m_type != SSARegisterListLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	if (m_usage == OutputSSARegistersLowLevelOperandUsage)
		return m_instr.GetRawOperandAsExpr(m_operandIndex).GetRawOperandAsSSARegisterList(1);
	if (m_usage == ParameterSSARegistersLowLevelOperandUsage)
		return m_instr.GetRawOperandAsExpr(m_operandIndex).GetRawOperandAsSSARegisterList(0);
	return m_instr.GetRawOperandAsSSARegisterList(m_operandIndex);
}


LowLevelILSSAFlagList LowLevelILOperand::GetSSAFlagList() const
{
	if (m_type != SSAFlagListLowLevelOperand)
		throw LowLevelILInstructionAccessException();
	return m_instr.GetRawOperandAsSSAFlagList(m_operandIndex);
}


const LowLevelILOperand LowLevelILOperandList::ListIterator::operator*()
{
	LowLevelILOperandUsage usage = *pos;
	auto i = owner->m_operandIndexMap.find(usage);
	if (i == owner->m_operandIndexMap.end())
		throw LowLevelILInstructionAccessException();
	return LowLevelILOperand(owner->m_instr, usage, i->second);
}


LowLevelILOperandList::LowLevelILOperandList(const LowLevelILInstruction& instr,
	const vector<LowLevelILOperandUsage>& usageList,
	const unordered_map<LowLevelILOperandUsage, size_t>& operandIndexMap):
	m_instr(instr), m_usageList(usageList), m_operandIndexMap(operandIndexMap)
{
}


LowLevelILOperandList::const_iterator LowLevelILOperandList::begin() const
{
	const_iterator result;
	result.owner = this;
	result.pos = m_usageList.begin();
	return result;
}


LowLevelILOperandList::const_iterator LowLevelILOperandList::end() const
{
	const_iterator result;
	result.owner = this;
	result.pos = m_usageList.end();
	return result;
}


size_t LowLevelILOperandList::size() const
{
	return m_usageList.size();
}


const LowLevelILOperand LowLevelILOperandList::operator[](size_t i) const
{
	LowLevelILOperandUsage usage = m_usageList[i];
	auto indexMap = m_operandIndexMap.find(usage);
	if (indexMap == m_operandIndexMap.end())
		throw LowLevelILInstructionAccessException();
	return LowLevelILOperand(m_instr, usage, indexMap->second);
}


LowLevelILOperandList::operator vector<LowLevelILOperand>() const
{
	vector<LowLevelILOperand> result;
	for (auto& i : *this)
		result.push_back(i);
	return result;
}


LowLevelILInstruction::LowLevelILInstruction()
{
	operation = LLIL_UNDEF;
	sourceOperand = BN_INVALID_OPERAND;
	size = 0;
	flags = 0;
	address = 0;
	function = nullptr;
	exprIndex = BN_INVALID_EXPR;
	instructionIndex = BN_INVALID_EXPR;
}


LowLevelILInstruction::LowLevelILInstruction(LowLevelILFunction* func,
	const BNLowLevelILInstruction& instr, size_t expr, size_t instrIdx)
{
	operation = instr.operation;
	sourceOperand = instr.sourceOperand;
	size = instr.size;
	flags = instr.flags;
	operands[0] = instr.operands[0];
	operands[1] = instr.operands[1];
	operands[2] = instr.operands[2];
	operands[3] = instr.operands[3];
	address = instr.address;
	function = func;
	exprIndex = expr;
	instructionIndex = instrIdx;
}


LowLevelILInstruction::LowLevelILInstruction(const LowLevelILInstructionBase& instr)
{
	operation = instr.operation;
	sourceOperand = instr.sourceOperand;
	size = instr.size;
	flags = instr.flags;
	operands[0] = instr.operands[0];
	operands[1] = instr.operands[1];
	operands[2] = instr.operands[2];
	operands[3] = instr.operands[3];
	address = instr.address;
	function = instr.function;
	exprIndex = instr.exprIndex;
	instructionIndex = instr.instructionIndex;
}


LowLevelILOperandList LowLevelILInstructionBase::GetOperands() const
{
	auto usage = operationOperandUsage.find(operation);
	if (usage == operationOperandUsage.end())
		throw LowLevelILInstructionAccessException();
	auto operandIndex = operationOperandIndex.find(operation);
	if (operandIndex == operationOperandIndex.end())
		throw LowLevelILInstructionAccessException();
	return LowLevelILOperandList(*(const LowLevelILInstruction*)this, usage->second, operandIndex->second);
}


uint64_t LowLevelILInstructionBase::GetRawOperandAsInteger(size_t operand) const
{
	return operands[operand];
}


size_t LowLevelILInstructionBase::GetRawOperandAsIndex(size_t operand) const
{
	return (size_t)operands[operand];
}


uint32_t LowLevelILInstructionBase::GetRawOperandAsRegister(size_t operand) const
{
	return (uint32_t)operands[operand];
}


BNLowLevelILFlagCondition LowLevelILInstructionBase::GetRawOperandAsFlagCondition(size_t operand) const
{
	return (BNLowLevelILFlagCondition)operands[operand];
}


LowLevelILInstruction LowLevelILInstructionBase::GetRawOperandAsExpr(size_t operand) const
{
	return LowLevelILInstruction(function, function->GetRawExpr(operands[operand]), operands[operand], instructionIndex);
}


SSARegister LowLevelILInstructionBase::GetRawOperandAsSSARegister(size_t operand) const
{
	return SSARegister((uint32_t)operands[operand], (size_t)operands[operand + 1]);
}


SSAFlag LowLevelILInstructionBase::GetRawOperandAsSSAFlag(size_t operand) const
{
	return SSAFlag((uint32_t)operands[operand], (size_t)operands[operand + 1]);
}


LowLevelILIndexList LowLevelILInstructionBase::GetRawOperandAsIndexList(size_t operand) const
{
	return LowLevelILIndexList(function, function->GetRawExpr(operands[operand + 1]), operands[operand]);
}


LowLevelILSSARegisterList LowLevelILInstructionBase::GetRawOperandAsSSARegisterList(size_t operand) const
{
	return LowLevelILSSARegisterList(function, function->GetRawExpr(operands[operand + 1]), operands[operand]);
}


LowLevelILSSAFlagList LowLevelILInstructionBase::GetRawOperandAsSSAFlagList(size_t operand) const
{
	return LowLevelILSSAFlagList(function, function->GetRawExpr(operands[operand + 1]), operands[operand]);
}


void LowLevelILInstructionBase::UpdateRawOperand(size_t operandIndex, ExprId value)
{
	operands[operandIndex] = value;
	function->UpdateInstructionOperand(exprIndex, operandIndex, value);
}


void LowLevelILInstructionBase::UpdateRawOperandAsSSARegisterList(size_t operandIndex, const vector<SSARegister>& regs)
{
	UpdateRawOperand(operandIndex, regs.size() * 2);
	UpdateRawOperand(operandIndex + 1, function->AddSSARegisterList(regs));
}


RegisterValue LowLevelILInstructionBase::GetValue() const
{
	return function->GetExprValue(*(const LowLevelILInstruction*)this);
}


PossibleValueSet LowLevelILInstructionBase::GetPossibleValues() const
{
	return function->GetPossibleExprValues(*(const LowLevelILInstruction*)this);
}


RegisterValue LowLevelILInstructionBase::GetRegisterValue(uint32_t reg)
{
	return function->GetRegisterValueAtInstruction(reg, instructionIndex);
}


RegisterValue LowLevelILInstructionBase::GetRegisterValueAfter(uint32_t reg)
{
	return function->GetRegisterValueAfterInstruction(reg, instructionIndex);
}


PossibleValueSet LowLevelILInstructionBase::GetPossibleRegisterValues(uint32_t reg)
{
	return function->GetPossibleRegisterValuesAtInstruction(reg, instructionIndex);
}


PossibleValueSet LowLevelILInstructionBase::GetPossibleRegisterValuesAfter(uint32_t reg)
{
	return function->GetPossibleRegisterValuesAfterInstruction(reg, instructionIndex);
}


RegisterValue LowLevelILInstructionBase::GetFlagValue(uint32_t flag)
{
	return function->GetFlagValueAtInstruction(flag, instructionIndex);
}


RegisterValue LowLevelILInstructionBase::GetFlagValueAfter(uint32_t flag)
{
	return function->GetFlagValueAfterInstruction(flag, instructionIndex);
}


PossibleValueSet LowLevelILInstructionBase::GetPossibleFlagValues(uint32_t flag)
{
	return function->GetPossibleFlagValuesAtInstruction(flag, instructionIndex);
}


PossibleValueSet LowLevelILInstructionBase::GetPossibleFlagValuesAfter(uint32_t flag)
{
	return function->GetPossibleFlagValuesAfterInstruction(flag, instructionIndex);
}


RegisterValue LowLevelILInstructionBase::GetStackContents(int32_t offset, size_t len)
{
	return function->GetStackContentsAtInstruction(offset, len, instructionIndex);
}


RegisterValue LowLevelILInstructionBase::GetStackContentsAfter(int32_t offset, size_t len)
{
	return function->GetStackContentsAfterInstruction(offset, len, instructionIndex);
}


PossibleValueSet LowLevelILInstructionBase::GetPossibleStackContents(int32_t offset, size_t len)
{
	return function->GetPossibleStackContentsAtInstruction(offset, len, instructionIndex);
}


PossibleValueSet LowLevelILInstructionBase::GetPossibleStackContentsAfter(int32_t offset, size_t len)
{
	return function->GetPossibleStackContentsAfterInstruction(offset, len, instructionIndex);
}


size_t LowLevelILInstructionBase::GetSSAInstructionIndex() const
{
	return function->GetSSAInstructionIndex(instructionIndex);
}


size_t LowLevelILInstructionBase::GetNonSSAInstructionIndex() const
{
	return function->GetNonSSAInstructionIndex(instructionIndex);
}


size_t LowLevelILInstructionBase::GetSSAExprIndex() const
{
	return function->GetSSAExprIndex(exprIndex);
}


size_t LowLevelILInstructionBase::GetNonSSAExprIndex() const
{
	return function->GetNonSSAExprIndex(exprIndex);
}


LowLevelILInstruction LowLevelILInstructionBase::GetSSAForm() const
{
	Ref<LowLevelILFunction> ssa = function->GetSSAForm().GetPtr();
	if (!ssa)
		return *this;
	size_t expr = GetSSAExprIndex();
	size_t instr = GetSSAInstructionIndex();
	return LowLevelILInstruction(ssa, ssa->GetRawExpr(expr), expr, instr);
}


LowLevelILInstruction LowLevelILInstructionBase::GetNonSSAForm() const
{
	Ref<LowLevelILFunction> nonSsa = function->GetNonSSAForm();
	if (!nonSsa)
		return *this;
	size_t expr = GetNonSSAExprIndex();
	size_t instr = GetNonSSAInstructionIndex();
	return LowLevelILInstruction(nonSsa, nonSsa->GetRawExpr(expr), expr, instr);
}


size_t LowLevelILInstructionBase::GetMediumLevelILInstructionIndex() const
{
	return function->GetMediumLevelILInstructionIndex(instructionIndex);
}


size_t LowLevelILInstructionBase::GetMediumLevelILExprIndex() const
{
	return function->GetMediumLevelILExprIndex(exprIndex);
}


size_t LowLevelILInstructionBase::GetMappedMediumLevelILInstructionIndex() const
{
	return function->GetMappedMediumLevelILInstructionIndex(instructionIndex);
}


size_t LowLevelILInstructionBase::GetMappedMediumLevelILExprIndex() const
{
	return function->GetMappedMediumLevelILExprIndex(exprIndex);
}


bool LowLevelILInstructionBase::HasMediumLevelIL() const
{
	Ref<MediumLevelILFunction> func = function->GetMediumLevelIL();
	if (!func)
		return false;
	return GetMediumLevelILExprIndex() < func->GetExprCount();
}


bool LowLevelILInstructionBase::HasMappedMediumLevelIL() const
{
	Ref<MediumLevelILFunction> func = function->GetMappedMediumLevelIL();
	if (!func)
		return false;
	return GetMappedMediumLevelILExprIndex() < func->GetExprCount();
}


MediumLevelILInstruction LowLevelILInstructionBase::GetMediumLevelIL() const
{
	Ref<MediumLevelILFunction> func = function->GetMediumLevelIL();
	if (!func)
		throw MediumLevelILInstructionAccessException();
	size_t expr = GetMediumLevelILExprIndex();
	if (expr >= func->GetExprCount())
		throw MediumLevelILInstructionAccessException();
	return func->GetExpr(expr);
}


MediumLevelILInstruction LowLevelILInstructionBase::GetMappedMediumLevelIL() const
{
	Ref<MediumLevelILFunction> func = function->GetMappedMediumLevelIL();
	if (!func)
		throw MediumLevelILInstructionAccessException();
	size_t expr = GetMappedMediumLevelILExprIndex();
	if (expr >= func->GetExprCount())
		throw MediumLevelILInstructionAccessException();
	return func->GetExpr(expr);
}


void LowLevelILInstructionBase::Replace(ExprId expr)
{
	function->ReplaceExpr(exprIndex, expr);
}


void LowLevelILInstruction::VisitExprs(const std::function<bool(const LowLevelILInstruction& expr)>& func) const
{
	if (!func(*this))
		return;
	switch (operation)
	{
	case LLIL_SET_REG:
		GetSourceExpr<LLIL_SET_REG>().VisitExprs(func);
		break;
	case LLIL_SET_REG_SPLIT:
		GetSourceExpr<LLIL_SET_REG_SPLIT>().VisitExprs(func);
		break;
	case LLIL_SET_REG_SSA:
		GetSourceExpr<LLIL_SET_REG_SSA>().VisitExprs(func);
		break;
	case LLIL_SET_REG_SSA_PARTIAL:
		GetSourceExpr<LLIL_SET_REG_SSA_PARTIAL>().VisitExprs(func);
		break;
	case LLIL_SET_REG_SPLIT_SSA:
		GetSourceExpr<LLIL_SET_REG_SPLIT_SSA>().VisitExprs(func);
		break;
	case LLIL_SET_FLAG:
		GetSourceExpr<LLIL_SET_FLAG>().VisitExprs(func);
		break;
	case LLIL_SET_FLAG_SSA:
		GetSourceExpr<LLIL_SET_FLAG_SSA>().VisitExprs(func);
		break;
	case LLIL_LOAD:
		GetSourceExpr<LLIL_LOAD>().VisitExprs(func);
		break;
	case LLIL_LOAD_SSA:
		GetSourceExpr<LLIL_LOAD_SSA>().VisitExprs(func);
		break;
	case LLIL_STORE:
		GetDestExpr<LLIL_STORE>().VisitExprs(func);
		GetSourceExpr<LLIL_STORE>().VisitExprs(func);
		break;
	case LLIL_STORE_SSA:
		GetDestExpr<LLIL_STORE_SSA>().VisitExprs(func);
		GetSourceExpr<LLIL_STORE_SSA>().VisitExprs(func);
		break;
	case LLIL_JUMP:
		GetDestExpr<LLIL_JUMP>().VisitExprs(func);
		break;
	case LLIL_JUMP_TO:
		GetDestExpr<LLIL_JUMP_TO>().VisitExprs(func);
		break;
	case LLIL_IF:
		GetConditionExpr<LLIL_IF>().VisitExprs(func);
		break;
	case LLIL_CALL:
		GetDestExpr<LLIL_CALL>().VisitExprs(func);
		break;
	case LLIL_CALL_STACK_ADJUST:
		GetDestExpr<LLIL_CALL_STACK_ADJUST>().VisitExprs(func);
		break;
	case LLIL_CALL_SSA:
		GetDestExpr<LLIL_CALL_SSA>().VisitExprs(func);
		break;
	case LLIL_RET:
		GetDestExpr<LLIL_RET>().VisitExprs(func);
		break;
	case LLIL_PUSH:
	case LLIL_NEG:
	case LLIL_NOT:
	case LLIL_SX:
	case LLIL_ZX:
	case LLIL_LOW_PART:
	case LLIL_BOOL_TO_INT:
	case LLIL_UNIMPL_MEM:
		AsOneOperand().GetSourceExpr().VisitExprs(func);
		break;
	case LLIL_ADD:
	case LLIL_SUB:
	case LLIL_AND:
	case LLIL_OR:
	case LLIL_XOR:
	case LLIL_LSL:
	case LLIL_LSR:
	case LLIL_ASR:
	case LLIL_ROL:
	case LLIL_ROR:
	case LLIL_MUL:
	case LLIL_MULU_DP:
	case LLIL_MULS_DP:
	case LLIL_DIVU:
	case LLIL_DIVS:
	case LLIL_MODU:
	case LLIL_MODS:
	case LLIL_CMP_E:
	case LLIL_CMP_NE:
	case LLIL_CMP_SLT:
	case LLIL_CMP_ULT:
	case LLIL_CMP_SLE:
	case LLIL_CMP_ULE:
	case LLIL_CMP_SGE:
	case LLIL_CMP_UGE:
	case LLIL_CMP_SGT:
	case LLIL_CMP_UGT:
	case LLIL_TEST_BIT:
	case LLIL_ADD_OVERFLOW:
		AsTwoOperand().GetLeftExpr().VisitExprs(func);
		AsTwoOperand().GetRightExpr().VisitExprs(func);
		break;
	case LLIL_ADC:
	case LLIL_SBB:
	case LLIL_RLC:
	case LLIL_RRC:
		AsTwoOperandWithCarry().GetLeftExpr().VisitExprs(func);
		AsTwoOperandWithCarry().GetRightExpr().VisitExprs(func);
		AsTwoOperandWithCarry().GetCarryExpr().VisitExprs(func);
		break;
	case LLIL_DIVU_DP:
	case LLIL_DIVS_DP:
	case LLIL_MODU_DP:
	case LLIL_MODS_DP:
		AsDoublePrecision().GetHighExpr().VisitExprs(func);
		AsDoublePrecision().GetLowExpr().VisitExprs(func);
		AsDoublePrecision().GetRightExpr().VisitExprs(func);
		break;
	default:
		break;
	}
}


ExprId LowLevelILInstruction::CopyTo(LowLevelILFunction* dest) const
{
	return CopyTo(dest, [&](const LowLevelILInstruction& subExpr) {
		return subExpr.CopyTo(dest);
	});
}


ExprId LowLevelILInstruction::CopyTo(LowLevelILFunction* dest,
	const std::function<ExprId(const LowLevelILInstruction& subExpr)>& subExprHandler) const
{
	vector<BNLowLevelILLabel*> labelList;
	BNLowLevelILLabel* labelA;
	BNLowLevelILLabel* labelB;
	switch (operation)
	{
	case LLIL_NOP:
		return dest->Nop();
	case LLIL_SET_REG:
		return dest->SetRegister(size, GetDestRegister<LLIL_SET_REG>(),
			subExprHandler(GetSourceExpr<LLIL_SET_REG>()), flags, *this);
	case LLIL_SET_REG_SPLIT:
		return dest->SetRegisterSplit(size, GetHighRegister<LLIL_SET_REG_SPLIT>(), GetLowRegister<LLIL_SET_REG_SPLIT>(),
			subExprHandler(GetSourceExpr<LLIL_SET_REG_SPLIT>()), flags, *this);
	case LLIL_SET_REG_SSA:
		return dest->SetRegisterSSA(size, GetDestSSARegister<LLIL_SET_REG_SSA>(),
			subExprHandler(GetSourceExpr<LLIL_SET_REG_SSA>()), *this);
	case LLIL_SET_REG_SSA_PARTIAL:
		return dest->SetRegisterSSAPartial(size, GetDestSSARegister<LLIL_SET_REG_SSA_PARTIAL>(),
			GetPartialRegister<LLIL_SET_REG_SSA_PARTIAL>(),
			subExprHandler(GetSourceExpr<LLIL_SET_REG_SSA_PARTIAL>()), *this);
	case LLIL_SET_REG_SPLIT_SSA:
		return dest->SetRegisterSplitSSA(size, GetHighSSARegister<LLIL_SET_REG_SPLIT_SSA>(),
			GetLowSSARegister<LLIL_SET_REG_SPLIT_SSA>(),
			subExprHandler(GetSourceExpr<LLIL_SET_REG_SPLIT_SSA>()), *this);
	case LLIL_SET_FLAG:
		return dest->SetFlag(GetDestFlag<LLIL_SET_FLAG>(), subExprHandler(GetSourceExpr<LLIL_SET_FLAG>()), *this);
	case LLIL_SET_FLAG_SSA:
		return dest->SetFlagSSA(GetDestSSAFlag<LLIL_SET_FLAG_SSA>(),
			subExprHandler(GetSourceExpr<LLIL_SET_FLAG_SSA>()), *this);
	case LLIL_LOAD:
		return dest->Load(size, subExprHandler(GetSourceExpr<LLIL_LOAD>()), flags, *this);
	case LLIL_LOAD_SSA:
		return dest->LoadSSA(size, subExprHandler(GetSourceExpr<LLIL_LOAD_SSA>()),
			GetSourceMemoryVersion<LLIL_LOAD_SSA>(), *this);
	case LLIL_STORE:
		return dest->Store(size, subExprHandler(GetDestExpr<LLIL_STORE>()),
			subExprHandler(GetSourceExpr<LLIL_STORE>()), flags, *this);
	case LLIL_STORE_SSA:
		return dest->StoreSSA(size, subExprHandler(GetDestExpr<LLIL_STORE_SSA>()),
			subExprHandler(GetSourceExpr<LLIL_STORE_SSA>()),
			GetDestMemoryVersion<LLIL_STORE_SSA>(), GetSourceMemoryVersion<LLIL_STORE_SSA>(), *this);
	case LLIL_REG:
		return dest->Register(size, GetSourceRegister<LLIL_REG>(), *this);
	case LLIL_REG_SSA:
		return dest->RegisterSSA(size, GetSourceSSARegister<LLIL_REG_SSA>(), *this);
	case LLIL_REG_SSA_PARTIAL:
		return dest->RegisterSSAPartial(size, GetSourceSSARegister<LLIL_REG_SSA_PARTIAL>(),
			GetPartialRegister<LLIL_REG_SSA_PARTIAL>(), *this);
	case LLIL_FLAG:
		return dest->Flag(GetSourceFlag<LLIL_FLAG>(), *this);
	case LLIL_FLAG_SSA:
		return dest->FlagSSA(GetSourceSSAFlag<LLIL_FLAG_SSA>(), *this);
	case LLIL_FLAG_BIT:
		return dest->FlagBit(size, GetSourceFlag<LLIL_FLAG_BIT>(), GetBitIndex<LLIL_FLAG_BIT>(), *this);
	case LLIL_FLAG_BIT_SSA:
		return dest->FlagBitSSA(size, GetSourceSSAFlag<LLIL_FLAG_BIT_SSA>(), GetBitIndex<LLIL_FLAG_BIT_SSA>(), *this);
	case LLIL_JUMP:
		return dest->Jump(subExprHandler(GetDestExpr<LLIL_JUMP>()), *this);
	case LLIL_CALL:
		return dest->Call(subExprHandler(GetDestExpr<LLIL_CALL>()), *this);
	case LLIL_CALL_STACK_ADJUST:
		return dest->CallStackAdjust(subExprHandler(GetDestExpr<LLIL_CALL_STACK_ADJUST>()),
			GetStackAdjustment<LLIL_CALL_STACK_ADJUST>(), *this);
	case LLIL_RET:
		return dest->Return(subExprHandler(GetDestExpr<LLIL_RET>()), *this);
	case LLIL_JUMP_TO:
		for (auto target : GetTargetList<LLIL_JUMP_TO>())
		{
			labelA = dest->GetLabelForSourceInstruction(target);
			if (!labelA)
				return dest->Jump(subExprHandler(GetDestExpr<LLIL_JUMP_TO>()), *this);
			labelList.push_back(labelA);
		}
		return dest->JumpTo(subExprHandler(GetDestExpr<LLIL_JUMP_TO>()), labelList, *this);
	case LLIL_GOTO:
		labelA = dest->GetLabelForSourceInstruction(GetTarget<LLIL_GOTO>());
		if (!labelA)
		{
			return dest->Jump(dest->ConstPointer(function->GetArchitecture()->GetAddressSize(),
				function->GetInstruction(GetTarget<LLIL_GOTO>()).address), *this);
		}
		return dest->Goto(*labelA, *this);
	case LLIL_IF:
		labelA = dest->GetLabelForSourceInstruction(GetTrueTarget<LLIL_IF>());
		labelB = dest->GetLabelForSourceInstruction(GetFalseTarget<LLIL_IF>());
		if ((!labelA) || (!labelB))
			return dest->Undefined(*this);
		return dest->If(subExprHandler(GetConditionExpr<LLIL_IF>()), *labelA, *labelB, *this);
	case LLIL_FLAG_COND:
		return dest->FlagCondition(GetFlagCondition<LLIL_FLAG_COND>(), *this);
	case LLIL_TRAP:
		return dest->Trap(GetVector<LLIL_TRAP>(), *this);
	case LLIL_CALL_SSA:
		return dest->CallSSA(GetOutputSSARegisters<LLIL_CALL_SSA>(), subExprHandler(GetDestExpr<LLIL_CALL_SSA>()),
			GetParameterSSARegisters<LLIL_CALL_SSA>(), GetStackSSARegister<LLIL_CALL_SSA>(),
			GetDestMemoryVersion<LLIL_CALL_SSA>(), GetSourceMemoryVersion<LLIL_CALL_SSA>(), *this);
	case LLIL_SYSCALL_SSA:
		return dest->SystemCallSSA(GetOutputSSARegisters<LLIL_SYSCALL_SSA>(),
			GetParameterSSARegisters<LLIL_SYSCALL_SSA>(), GetStackSSARegister<LLIL_SYSCALL_SSA>(),
			GetDestMemoryVersion<LLIL_SYSCALL_SSA>(), GetSourceMemoryVersion<LLIL_SYSCALL_SSA>(), *this);
	case LLIL_REG_PHI:
		return dest->RegisterPhi(GetDestSSARegister<LLIL_REG_PHI>(), GetSourceSSARegisters<LLIL_REG_PHI>(), *this);
	case LLIL_FLAG_PHI:
		return dest->FlagPhi(GetDestSSAFlag<LLIL_FLAG_PHI>(), GetSourceSSAFlags<LLIL_FLAG_PHI>(), *this);
	case LLIL_MEM_PHI:
		return dest->MemoryPhi(GetDestMemoryVersion<LLIL_MEM_PHI>(), GetSourceMemoryVersions<LLIL_MEM_PHI>(), *this);
	case LLIL_CONST:
		return dest->Const(size, GetConstant<LLIL_CONST>(), *this);
	case LLIL_CONST_PTR:
		return dest->ConstPointer(size, GetConstant<LLIL_CONST_PTR>(), *this);
	case LLIL_POP:
	case LLIL_NORET:
	case LLIL_SYSCALL:
	case LLIL_BP:
	case LLIL_UNDEF:
	case LLIL_UNIMPL:
		return dest->AddExprWithLocation(operation, *this, size, flags);
	case LLIL_PUSH:
	case LLIL_NEG:
	case LLIL_NOT:
	case LLIL_SX:
	case LLIL_ZX:
	case LLIL_LOW_PART:
	case LLIL_BOOL_TO_INT:
	case LLIL_UNIMPL_MEM:
		return dest->AddExprWithLocation(operation, *this, size, flags,
			subExprHandler(AsOneOperand().GetSourceExpr()));
	case LLIL_ADD:
	case LLIL_SUB:
	case LLIL_AND:
	case LLIL_OR:
	case LLIL_XOR:
	case LLIL_LSL:
	case LLIL_LSR:
	case LLIL_ASR:
	case LLIL_ROL:
	case LLIL_ROR:
	case LLIL_MUL:
	case LLIL_MULU_DP:
	case LLIL_MULS_DP:
	case LLIL_DIVU:
	case LLIL_DIVS:
	case LLIL_MODU:
	case LLIL_MODS:
	case LLIL_CMP_E:
	case LLIL_CMP_NE:
	case LLIL_CMP_SLT:
	case LLIL_CMP_ULT:
	case LLIL_CMP_SLE:
	case LLIL_CMP_ULE:
	case LLIL_CMP_SGE:
	case LLIL_CMP_UGE:
	case LLIL_CMP_SGT:
	case LLIL_CMP_UGT:
	case LLIL_TEST_BIT:
	case LLIL_ADD_OVERFLOW:
		return dest->AddExprWithLocation(operation, *this, size, flags,
			subExprHandler(AsTwoOperand().GetLeftExpr()), subExprHandler(AsTwoOperand().GetRightExpr()));
	case LLIL_ADC:
	case LLIL_SBB:
	case LLIL_RLC:
	case LLIL_RRC:
		return dest->AddExprWithLocation(operation, *this, size, flags,
			subExprHandler(AsTwoOperandWithCarry().GetLeftExpr()),
			subExprHandler(AsTwoOperandWithCarry().GetRightExpr()),
			subExprHandler(AsTwoOperandWithCarry().GetCarryExpr()));
	case LLIL_DIVU_DP:
	case LLIL_DIVS_DP:
	case LLIL_MODU_DP:
	case LLIL_MODS_DP:
		return dest->AddExprWithLocation(operation, *this, size, flags,
			subExprHandler(AsDoublePrecision().GetHighExpr()),
			subExprHandler(AsDoublePrecision().GetLowExpr()),
			subExprHandler(AsDoublePrecision().GetRightExpr()));
	default:
		throw LowLevelILInstructionAccessException();
	}
}


bool LowLevelILInstruction::GetOperandIndexForUsage(LowLevelILOperandUsage usage, size_t& operandIndex) const
{
	auto operationIter = LowLevelILInstructionBase::operationOperandIndex.find(operation);
	if (operationIter == LowLevelILInstructionBase::operationOperandIndex.end())
		return false;
	auto usageIter = operationIter->second.find(usage);
	if (usageIter == operationIter->second.end())
		return false;
	operandIndex = usageIter->second;
	return true;
}


LowLevelILInstruction LowLevelILInstruction::GetSourceExpr() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(SourceExprLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex);
	throw LowLevelILInstructionAccessException();
}


uint32_t LowLevelILInstruction::GetSourceRegister() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(SourceRegisterLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsRegister(operandIndex);
	throw LowLevelILInstructionAccessException();
}


uint32_t LowLevelILInstruction::GetSourceFlag() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(SourceFlagLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsRegister(operandIndex);
	throw LowLevelILInstructionAccessException();
}


SSARegister LowLevelILInstruction::GetSourceSSARegister() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(SourceSSARegisterLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsSSARegister(operandIndex);
	throw LowLevelILInstructionAccessException();
}


SSAFlag LowLevelILInstruction::GetSourceSSAFlag() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(SourceSSAFlagLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsSSAFlag(operandIndex);
	throw LowLevelILInstructionAccessException();
}


LowLevelILInstruction LowLevelILInstruction::GetDestExpr() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(DestExprLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex);
	throw LowLevelILInstructionAccessException();
}


uint32_t LowLevelILInstruction::GetDestRegister() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(DestRegisterLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsRegister(operandIndex);
	throw LowLevelILInstructionAccessException();
}


uint32_t LowLevelILInstruction::GetDestFlag() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(DestFlagLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsRegister(operandIndex);
	throw LowLevelILInstructionAccessException();
}


SSARegister LowLevelILInstruction::GetDestSSARegister() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(DestSSARegisterLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsSSARegister(operandIndex);
	throw LowLevelILInstructionAccessException();
}


SSAFlag LowLevelILInstruction::GetDestSSAFlag() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(DestSSAFlagLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsSSAFlag(operandIndex);
	throw LowLevelILInstructionAccessException();
}


uint32_t LowLevelILInstruction::GetPartialRegister() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(PartialRegisterLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsRegister(operandIndex);
	throw LowLevelILInstructionAccessException();
}


SSARegister LowLevelILInstruction::GetStackSSARegister() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(StackSSARegisterLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex).GetRawOperandAsSSARegister(0);
	throw LowLevelILInstructionAccessException();
}


LowLevelILInstruction LowLevelILInstruction::GetLeftExpr() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(LeftExprLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex);
	throw LowLevelILInstructionAccessException();
}


LowLevelILInstruction LowLevelILInstruction::GetRightExpr() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(RightExprLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex);
	throw LowLevelILInstructionAccessException();
}


LowLevelILInstruction LowLevelILInstruction::GetCarryExpr() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(CarryExprLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex);
	throw LowLevelILInstructionAccessException();
}


LowLevelILInstruction LowLevelILInstruction::GetHighExpr() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(HighExprLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex);
	throw LowLevelILInstructionAccessException();
}


LowLevelILInstruction LowLevelILInstruction::GetLowExpr() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(LowExprLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex);
	throw LowLevelILInstructionAccessException();
}


LowLevelILInstruction LowLevelILInstruction::GetConditionExpr() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(ConditionExprLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex);
	throw LowLevelILInstructionAccessException();
}


uint32_t LowLevelILInstruction::GetHighRegister() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(HighRegisterLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsRegister(operandIndex);
	throw LowLevelILInstructionAccessException();
}


SSARegister LowLevelILInstruction::GetHighSSARegister() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(HighSSARegisterLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex).GetRawOperandAsSSARegister(0);
	throw LowLevelILInstructionAccessException();
}


uint32_t LowLevelILInstruction::GetLowRegister() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(LowRegisterLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsRegister(operandIndex);
	throw LowLevelILInstructionAccessException();
}


SSARegister LowLevelILInstruction::GetLowSSARegister() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(LowSSARegisterLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex).GetRawOperandAsSSARegister(0);
	throw LowLevelILInstructionAccessException();
}


int64_t LowLevelILInstruction::GetConstant() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(ConstantLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsInteger(operandIndex);
	throw LowLevelILInstructionAccessException();
}


int64_t LowLevelILInstruction::GetVector() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(VectorLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsInteger(operandIndex);
	throw LowLevelILInstructionAccessException();
}


size_t LowLevelILInstruction::GetStackAdjustment() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(StackAdjustmentLowLevelOperandUsage, operandIndex))
		return (size_t)GetRawOperandAsInteger(operandIndex);
	throw LowLevelILInstructionAccessException();
}


size_t LowLevelILInstruction::GetTarget() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(TargetLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsIndex(operandIndex);
	throw LowLevelILInstructionAccessException();
}


size_t LowLevelILInstruction::GetTrueTarget() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(TrueTargetLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsIndex(operandIndex);
	throw LowLevelILInstructionAccessException();
}


size_t LowLevelILInstruction::GetFalseTarget() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(FalseTargetLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsIndex(operandIndex);
	throw LowLevelILInstructionAccessException();
}


size_t LowLevelILInstruction::GetBitIndex() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(BitIndexLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsIndex(operandIndex);
	throw LowLevelILInstructionAccessException();
}


size_t LowLevelILInstruction::GetSourceMemoryVersion() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(SourceMemoryVersionLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsIndex(operandIndex);
	if (GetOperandIndexForUsage(StackMemoryVersionLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex).GetRawOperandAsIndex(2);
	throw LowLevelILInstructionAccessException();
}


size_t LowLevelILInstruction::GetDestMemoryVersion() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(DestMemoryVersionLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsIndex(operandIndex);
	if (GetOperandIndexForUsage(OutputMemoryVersionLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex).GetRawOperandAsIndex(0);
	throw LowLevelILInstructionAccessException();
}


BNLowLevelILFlagCondition LowLevelILInstruction::GetFlagCondition() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(FlagConditionLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsFlagCondition(operandIndex);
	throw LowLevelILInstructionAccessException();
}


LowLevelILSSARegisterList LowLevelILInstruction::GetOutputSSARegisters() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(OutputSSARegistersLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex).GetRawOperandAsSSARegisterList(1);
	throw LowLevelILInstructionAccessException();
}


LowLevelILSSARegisterList LowLevelILInstruction::GetParameterSSARegisters() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(ParameterSSARegistersLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsExpr(operandIndex).GetRawOperandAsSSARegisterList(0);
	throw LowLevelILInstructionAccessException();
}


LowLevelILSSARegisterList LowLevelILInstruction::GetSourceSSARegisters() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(SourceSSARegistersLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsSSARegisterList(operandIndex);
	throw LowLevelILInstructionAccessException();
}


LowLevelILSSAFlagList LowLevelILInstruction::GetSourceSSAFlags() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(SourceSSAFlagsLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsSSAFlagList(operandIndex);
	throw LowLevelILInstructionAccessException();
}


LowLevelILIndexList LowLevelILInstruction::GetSourceMemoryVersions() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(SourceMemoryVersionsLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsIndexList(operandIndex);
	throw LowLevelILInstructionAccessException();
}


LowLevelILIndexList LowLevelILInstruction::GetTargetList() const
{
	size_t operandIndex;
	if (GetOperandIndexForUsage(TargetListLowLevelOperandUsage, operandIndex))
		return GetRawOperandAsIndexList(operandIndex);
	throw LowLevelILInstructionAccessException();
}


ExprId LowLevelILFunction::Nop(const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_NOP, loc, 0, 0);
}


ExprId LowLevelILFunction::SetRegister(size_t size, uint32_t reg, ExprId val, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SET_REG, loc, size, flags, reg, val);
}


ExprId LowLevelILFunction::SetRegisterSplit(size_t size, uint32_t high, uint32_t low, ExprId val, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SET_REG_SPLIT, loc, size, flags, high, low, val);
}


ExprId LowLevelILFunction::SetRegisterSSA(size_t size, const SSARegister& reg, ExprId val,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SET_REG_SSA, loc, size, 0, reg.reg, reg.version, val);
}


ExprId LowLevelILFunction::SetRegisterSSAPartial(size_t size, const SSARegister& fullReg, uint32_t partialReg,
	ExprId val, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SET_REG_SSA_PARTIAL, loc, size, 0, fullReg.reg, fullReg.version, partialReg, val);
}


ExprId LowLevelILFunction::SetRegisterSplitSSA(size_t size, const SSARegister& high, const SSARegister& low,
	ExprId val, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SET_REG_SPLIT_SSA, loc, size, 0,
		AddExprWithLocation(LLIL_REG_SPLIT_DEST_SSA, loc, size, 0, high.reg, high.version),
		AddExprWithLocation(LLIL_REG_SPLIT_DEST_SSA, loc, size, 0, low.reg, low.version), val);
}


ExprId LowLevelILFunction::SetFlag(uint32_t flag, ExprId val, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SET_FLAG, loc, 0, 0, flag, val);
}


ExprId LowLevelILFunction::SetFlagSSA(const SSAFlag& flag, ExprId val, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SET_FLAG_SSA, loc, 0, 0, flag.flag, flag.version, val);
}


ExprId LowLevelILFunction::Load(size_t size, ExprId addr, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_LOAD, loc, size, flags, addr);
}


ExprId LowLevelILFunction::LoadSSA(size_t size, ExprId addr, size_t sourceMemoryVer,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_LOAD_SSA, loc, size, 0, addr, sourceMemoryVer);
}


ExprId LowLevelILFunction::Store(size_t size, ExprId addr, ExprId val, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_STORE, loc, size, flags, addr, val);
}


ExprId LowLevelILFunction::StoreSSA(size_t size, ExprId addr, ExprId val, size_t newMemoryVer, size_t prevMemoryVer,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_STORE_SSA, loc, size, 0, addr, newMemoryVer, prevMemoryVer, val);
}


ExprId LowLevelILFunction::Push(size_t size, ExprId val, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_PUSH, loc, size, flags, val);
}


ExprId LowLevelILFunction::Pop(size_t size, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_POP, loc, size, flags);
}


ExprId LowLevelILFunction::Register(size_t size, uint32_t reg, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_REG, loc, size, 0, reg);
}


ExprId LowLevelILFunction::RegisterSSA(size_t size, const SSARegister& reg,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_REG_SSA, loc, size, 0, reg.reg, reg.version);
}


ExprId LowLevelILFunction::RegisterSSAPartial(size_t size, const SSARegister& fullReg, uint32_t partialReg,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_REG_SSA_PARTIAL, loc, size, 0, fullReg.reg, fullReg.version, partialReg);
}


ExprId LowLevelILFunction::Const(size_t size, uint64_t val, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CONST, loc, size, 0, val);
}


ExprId LowLevelILFunction::ConstPointer(size_t size, uint64_t val, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CONST_PTR, loc, size, 0, val);
}


ExprId LowLevelILFunction::Flag(uint32_t flag, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_FLAG, loc, 0, 0, flag);
}


ExprId LowLevelILFunction::FlagSSA(const SSAFlag& flag, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_FLAG_SSA, loc, 0, 0, flag.flag, flag.version);
}


ExprId LowLevelILFunction::FlagBit(size_t size, uint32_t flag, uint32_t bitIndex, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_FLAG_BIT, loc, size, 0, flag, bitIndex);
}


ExprId LowLevelILFunction::FlagBitSSA(size_t size, const SSAFlag& flag, uint32_t bitIndex,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_FLAG_BIT_SSA, loc, size, 0, flag.flag, flag.version, bitIndex);
}


ExprId LowLevelILFunction::Add(size_t size, ExprId a, ExprId b, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_ADD, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::AddCarry(size_t size, ExprId a, ExprId b, ExprId carry, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_ADC, loc, size, flags, a, b, carry);
}


ExprId LowLevelILFunction::Sub(size_t size, ExprId a, ExprId b, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SUB, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::SubBorrow(size_t size, ExprId a, ExprId b, ExprId carry, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SBB, loc, size, flags, a, b, carry);
}


ExprId LowLevelILFunction::And(size_t size, ExprId a, ExprId b, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_AND, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::Or(size_t size, ExprId a, ExprId b, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_OR, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::Xor(size_t size, ExprId a, ExprId b, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_XOR, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::ShiftLeft(size_t size, ExprId a, ExprId b, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_LSL, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::LogicalShiftRight(size_t size, ExprId a, ExprId b, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_LSR, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::ArithShiftRight(size_t size, ExprId a, ExprId b, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_ASR, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::RotateLeft(size_t size, ExprId a, ExprId b, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_ROL, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::RotateLeftCarry(size_t size, ExprId a, ExprId b, ExprId carry, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_RLC, loc, size, flags, a, b, carry);
}


ExprId LowLevelILFunction::RotateRight(size_t size, ExprId a, ExprId b, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_ROR, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::RotateRightCarry(size_t size, ExprId a, ExprId b, ExprId carry, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_RRC, loc, size, flags, a, b, carry);
}


ExprId LowLevelILFunction::Mult(size_t size, ExprId a, ExprId b, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_MUL, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::MultDoublePrecUnsigned(size_t size, ExprId a, ExprId b, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_MULU_DP, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::MultDoublePrecSigned(size_t size, ExprId a, ExprId b, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_MULS_DP, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::DivUnsigned(size_t size, ExprId a, ExprId b, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_DIVU, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::DivDoublePrecUnsigned(size_t size, ExprId high, ExprId low, ExprId div, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_DIVU_DP, loc, size, flags, high, low, div);
}


ExprId LowLevelILFunction::DivSigned(size_t size, ExprId a, ExprId b, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_DIVS, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::DivDoublePrecSigned(size_t size, ExprId high, ExprId low, ExprId div, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_DIVS_DP, loc, size, flags, high, low, div);
}


ExprId LowLevelILFunction::ModUnsigned(size_t size, ExprId a, ExprId b, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_MODU, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::ModDoublePrecUnsigned(size_t size, ExprId high, ExprId low, ExprId div, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_MODU_DP, loc, size, flags, high, low, div);
}


ExprId LowLevelILFunction::ModSigned(size_t size, ExprId a, ExprId b, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_MODS, loc, size, flags, a, b);
}


ExprId LowLevelILFunction::ModDoublePrecSigned(size_t size, ExprId high, ExprId low, ExprId div, uint32_t flags,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_MODS_DP, loc, size, flags, high, low, div);
}


ExprId LowLevelILFunction::Neg(size_t size, ExprId a, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_NEG, loc, size, flags, a);
}


ExprId LowLevelILFunction::Not(size_t size, ExprId a, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_NOT, loc, size, flags, a);
}


ExprId LowLevelILFunction::SignExtend(size_t size, ExprId a, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SX, loc, size, flags, a);
}


ExprId LowLevelILFunction::ZeroExtend(size_t size, ExprId a, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_ZX, loc, size, flags, a);
}


ExprId LowLevelILFunction::LowPart(size_t size, ExprId a, uint32_t flags, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_LOW_PART, loc, size, flags, a);
}


ExprId LowLevelILFunction::Jump(ExprId dest, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_JUMP, loc, 0, 0, dest);
}


ExprId LowLevelILFunction::JumpTo(ExprId dest, const vector<BNLowLevelILLabel*>& targets,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_JUMP_TO, loc, 0, 0, dest, targets.size(), AddLabelList(targets));
}


ExprId LowLevelILFunction::Call(ExprId dest, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CALL, loc, 0, 0, dest);
}


ExprId LowLevelILFunction::CallStackAdjust(ExprId dest, size_t adjust, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CALL_STACK_ADJUST, loc, 0, 0, dest, adjust);
}


ExprId LowLevelILFunction::CallSSA(const vector<SSARegister>& output, ExprId dest, const vector<SSARegister>& params,
	const SSARegister& stack, size_t newMemoryVer, size_t prevMemoryVer, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CALL_SSA, loc, 0, 0,
		AddExprWithLocation(LLIL_CALL_OUTPUT_SSA, loc, 0, 0, newMemoryVer,
			output.size() * 2, AddSSARegisterList(output)), dest,
		AddExprWithLocation(LLIL_CALL_STACK_SSA, loc, 0, 0, stack.reg, stack.version, prevMemoryVer),
		AddExprWithLocation(LLIL_CALL_PARAM_SSA, loc, 0, 0,
			params.size() * 2, AddSSARegisterList(params)));
}


ExprId LowLevelILFunction::SystemCallSSA(const vector<SSARegister>& output, const vector<SSARegister>& params,
	const SSARegister& stack, size_t newMemoryVer, size_t prevMemoryVer, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SYSCALL_SSA, loc, 0, 0,
		AddExprWithLocation(LLIL_CALL_OUTPUT_SSA, loc, 0, 0, newMemoryVer,
			output.size() * 2, AddSSARegisterList(output)),
		AddExprWithLocation(LLIL_CALL_STACK_SSA, loc, 0, 0, stack.reg, stack.version, prevMemoryVer),
		AddExprWithLocation(LLIL_CALL_PARAM_SSA, loc, 0, 0,
			params.size() * 2, AddSSARegisterList(params)));
}


ExprId LowLevelILFunction::Return(size_t dest, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_RET, loc, 0, 0, dest);
}


ExprId LowLevelILFunction::NoReturn(const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_NORET, loc, 0, 0);
}


ExprId LowLevelILFunction::FlagCondition(BNLowLevelILFlagCondition cond, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_FLAG_COND, loc, 0, 0, (ExprId)cond);
}


ExprId LowLevelILFunction::CompareEqual(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CMP_E, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::CompareNotEqual(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CMP_NE, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::CompareSignedLessThan(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CMP_SLT, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::CompareUnsignedLessThan(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CMP_ULT, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::CompareSignedLessEqual(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CMP_SLE, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::CompareUnsignedLessEqual(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CMP_ULE, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::CompareSignedGreaterEqual(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CMP_SGE, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::CompareUnsignedGreaterEqual(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CMP_UGE, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::CompareSignedGreaterThan(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CMP_SGT, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::CompareUnsignedGreaterThan(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_CMP_UGT, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::TestBit(size_t size, ExprId a, ExprId b, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_TEST_BIT, loc, size, 0, a, b);
}


ExprId LowLevelILFunction::BoolToInt(size_t size, ExprId a, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_BOOL_TO_INT, loc, size, 0, a);
}


ExprId LowLevelILFunction::SystemCall(const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_SYSCALL, loc, 0, 0);
}


ExprId LowLevelILFunction::Breakpoint(const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_BP, loc, 0, 0);
}


ExprId LowLevelILFunction::Trap(uint32_t num, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_TRAP, loc, 0, 0, num);
}


ExprId LowLevelILFunction::Undefined(const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_UNDEF, loc, 0, 0);
}


ExprId LowLevelILFunction::Unimplemented(const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_UNIMPL, loc, 0, 0);
}


ExprId LowLevelILFunction::UnimplementedMemoryRef(size_t size, ExprId addr, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_UNIMPL_MEM, loc, size, 0, addr);
}


ExprId LowLevelILFunction::RegisterPhi(const SSARegister& dest, const vector<SSARegister>& sources,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_REG_PHI, loc, 0, 0, dest.reg, dest.version,
		sources.size() * 2, AddSSARegisterList(sources));
}


ExprId LowLevelILFunction::FlagPhi(const SSAFlag& dest, const vector<SSAFlag>& sources,
	const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_FLAG_PHI, loc, 0, 0, dest.flag, dest.version,
		sources.size() * 2, AddSSAFlagList(sources));
}


ExprId LowLevelILFunction::MemoryPhi(size_t dest, const vector<size_t>& sources, const ILSourceLocation& loc)
{
	return AddExprWithLocation(LLIL_MEM_PHI, loc, 0, 0, dest, sources.size(), AddIndexList(sources));
}
