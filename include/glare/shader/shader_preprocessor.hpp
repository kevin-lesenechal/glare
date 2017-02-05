#pragma once

#include <string>
#include <map>

namespace glare {

class ShaderSourceLoaderInterface;

class ShaderPreprocessor
{
public:
    explicit ShaderPreprocessor(ShaderSourceLoaderInterface& source_loader);

    std::string preprocess(const std::string& source);

    void define(const std::string& key, const std::string& value);
    void undefine(const std::string& key);

private:
    ShaderSourceLoaderInterface& m_source_loader;
    std::map<std::string, std::string> m_defines;
};

} // ns glare
