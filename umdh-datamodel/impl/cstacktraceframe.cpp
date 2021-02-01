#include "cstacktraceframe.h"

#pragma warning(push, 0)
#include <cassert>
#pragma warning(pop)

using namespace datamodel;


CStackTraceFrame::CStackTraceFrame(const gui::IObjectFactory *pObjectFactory, std::wstring typeId)
    :
      m_pObjectFactory(pObjectFactory),
      m_typeId(std::move(typeId))
{
    assert(m_pObjectFactory);
    static_assert(std::is_copy_constructible<CopyableState>::value,
            "CStackTraceFrame.CopyableState is not copyable!");
}

std::wstring CStackTraceFrame::type() const
{
    return m_typeId;
}

gui::unique_ptr<gui::IClonable> CStackTraceFrame::clone() const
{
    auto pStackTraceFrame = gui::dynamic_unique_cast<CStackTraceFrame>(m_pObjectFactory->get(type()));
    pStackTraceFrame->m_pObjectFactory = m_pObjectFactory;
    pStackTraceFrame->m_copyableState = m_copyableState;
    return std::move(pStackTraceFrame);
}

void CStackTraceFrame::setModule(std::wstring module)
{
    m_copyableState.module = std::move(module);
}

std::wstring CStackTraceFrame::getModule() const
{
    return m_copyableState.module;
}

void CStackTraceFrame::setFunction(std::wstring function)
{
    m_copyableState.function = std::move(function);
}

std::wstring CStackTraceFrame::getFunction() const
{
    return m_copyableState.function;
}

void CStackTraceFrame::setOffset(size_t offset)
{
    m_copyableState.offset = offset;
}

size_t CStackTraceFrame::getOffset() const
{
    return m_copyableState.offset;
}

void CStackTraceFrame::setSource(std::wstring source)
{
    m_copyableState.source = std::move(source);
}

std::wstring CStackTraceFrame::getSource() const
{
    return m_copyableState.source;
}

void CStackTraceFrame::setSourceLine(size_t sourceLine)
{
    m_copyableState.sourceLine = sourceLine;
}

size_t CStackTraceFrame::getSourceLine() const
{
    return m_copyableState.sourceLine;
}

void CStackTraceFrame::setUnknownAddress(std::wstring unknownAddress)
{
    m_copyableState.unknownAddress = std::move(unknownAddress);
}

std::wstring CStackTraceFrame::getUnknownAddress() const
{
    return m_copyableState.unknownAddress;
}
