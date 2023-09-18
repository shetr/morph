#include "Storage.hpp"

#include <filesystem>
#include <fstream>

namespace Morph {

ResourceStorage::ResourceStorage(string resSeparator)
    : m_resSeparator(resSeparator)
{   
}
ResourceStorage::ResourceStorage(const unord_map<string, string>& resFoldersToPaths, string resSeparator)
    : m_resFoldersToPaths(resFoldersToPaths), m_resSeparator(resSeparator)
{
}

void ResourceStorage::InsertResFolder(const string& name, const string& path)
{
    m_resFoldersToPaths.insert_or_assign(name, path);
}
opt<string> ResourceStorage::GetResFolderPath(const string& resFolder) const
{
    auto folderPathPairIt = m_resFoldersToPaths.find(resFolder);
    if(folderPathPairIt != m_resFoldersToPaths.end()) {
        return folderPathPairIt->second;
    }
    return {};
}
opt<string> ResourceStorage::GetPath(const string& resFolderRelPath) const
{
    string::size_type sepPos = resFolderRelPath.find(m_resSeparator);
    if (sepPos != string::npos) {
        string resFolder = resFolderRelPath.substr(0, sepPos);
        string relPath = resFolderRelPath.substr(sepPos + 1);
        return GetPath(resFolder, relPath);
    }
    return {};
}
opt<string> ResourceStorage::GetPath(const string& resFolder, const string& relPath) const
{
    opt<string> resFolderPathOpt = GetResFolderPath(resFolder);
    if(resFolderPathOpt) {
        string& resFolderPath = resFolderPathOpt.value();
        string fullPathStr = resFolderPath + relPath;
        return fullPathStr;
    }
    return {};
}

opt<string> ResourceStorage::GetResPathOrNormalPath(const string& resFolderRelPathOrNormalPath) const
{
    string::size_type sepPos = resFolderRelPathOrNormalPath.find(m_resSeparator);
    if (sepPos != string::npos) {
        string resFolder = resFolderRelPathOrNormalPath.substr(0, sepPos);
        string relPath = resFolderRelPathOrNormalPath.substr(sepPos + 1);
        return GetPath(resFolder, relPath);
    }
    return resFolderRelPathOrNormalPath;
}

opt<string> ResourceStorage::ReadFileRes(const string& resFolderRelPath) const
{
    opt<string> maybePath = GetPath(resFolderRelPath);
    if(maybePath) {
        return ReadFile(maybePath.value());
    }
    return {};
}
bool ResourceStorage::WriteToFileRes(const string& resFolderRelPath, const string& contents) const
{
    opt<string> maybePath = GetPath(resFolderRelPath);
    if(maybePath) {
        return WriteToFile(maybePath.value(), contents);
    }
    return false;
}

opt<string> ResourceStorage::ReadFileResOrNormal(const string& resFolderRelPathOrNormalPath) const
{
    opt<string> maybePath = GetResPathOrNormalPath(resFolderRelPathOrNormalPath);
    if(maybePath) {
        return ReadFile(maybePath.value());
    }
    return {};
}
bool ResourceStorage::WriteToFileResOrNormal(const string& resFolderRelPathOrNormalPath, const string& contents) const
{
    opt<string> maybePath = GetResPathOrNormalPath(resFolderRelPathOrNormalPath);
    if(maybePath) {
        return WriteToFile(maybePath.value(), contents);
    }
    return false;
}


opt<string> ResourceStorage::ReadFile(const string& filePathStr)
{
    std::filesystem::path filePath(filePathStr);
    std::ifstream file(filePathStr, std::ios::binary);
    if(file) {
        std::uintmax_t fileSize = std::filesystem::file_size(filePath);
        std::string contents;
        contents.reserve(fileSize);
        contents.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return contents;
    }
    return {};
}
bool ResourceStorage::WriteToFile(const string& filePathStr, const string& contents)
{
    std::ofstream file(filePathStr, std::ios::binary);
    if (file.is_open()) {
        file << contents;
        file.close();
        return true;
    }
    return false;
}


}