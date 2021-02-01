#ifndef CFORWARDPARSER_H
#define CFORWARDPARSER_H

#include "customuniqueptr.h"

#pragma warning(push, 0)
#include <string>
#include <vector>
#include <functional>
#pragma warning(pop)

namespace datamodelserializer
{
    class ITokenParser
    {
    public:
        virtual ~ITokenParser() = default;

        virtual size_t processStep(const std::wstring &stringData,
                                   size_t currentPos,
                                   bool &tokenStartInited,
                                   size_t &tokenStart) = 0;

        virtual void setTokenRepeat(long ntimes) = 0;
    };

    class BaseTokenParser : public ITokenParser
    {
    public:
        BaseTokenParser();
        void setStartTokenCondition(std::function<bool(const std::wstring&, size_t)> startTokenCondition);
        void setEndTokenCondition(std::function<bool(const std::wstring&, size_t)> endTokenCondition);
        void setTokenGetter(std::function<void(const std::wstring&, size_t, const std::wstring&)> tokenGetter);

        void setTokenRepeat(long ntimes) override;

    private:
        // ITokenParser
        virtual size_t processStep(const std::wstring &stringData, size_t currentPos, bool &tokenStartInited, size_t &tokenStart) override;

    private:
        std::function<bool(const std::wstring&, size_t)> m_startTokenCondition;
        std::function<bool(const std::wstring&, size_t)> m_endTokenCondition;
        std::function<void(const std::wstring&, size_t, const std::wstring&)> m_tokenGetter;
        long m_readNtimes;
    };

    void readString(const std::wstring &data, std::vector<gui::unique_ptr<datamodelserializer::ITokenParser>> tokensGetters);
}


#endif // CFORWARDPARSER_H
