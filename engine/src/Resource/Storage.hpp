
#ifndef MORPH_RESOURCE_STORAGE_HPP
#define MORPH_RESOURCE_STORAGE_HPP

#include <Core/Core.hpp>

namespace Morph {

class ResourceStorage
{
private:
    unord_map<string, string> m_resFoldersToPaths;
    string m_resSeparator;
public:
    ResourceStorage(string resSeparator = ":");
    ResourceStorage(const unord_map<string, string>& resFoldersToPaths, string resSeparator = ":");

    void InsertResFolder(const string& name, const string& path);
    opt<string> GetResFolderPath(const string& resFolder) const;
    opt<string> GetPath(const string& resFolderRelPath) const;
    opt<string> GetPath(const string& resFolder, const string& relPath) const;
    opt<string> GetResPathOrNormalPath(const string& resFolderRelPathOrNormalPath) const;

    opt<string> ReadFileRes(const string& resFolderRelPath) const;
    bool WriteToFileRes(const string& resFolderRelPath, const string& contents) const;

    opt<string> ReadFileResOrNormal(const string& resFolderRelPathOrNormalPath) const;
    bool WriteToFileResOrNormal(const string& resFolderRelPathOrNormalPath, const string& contents) const;

    static opt<string> ReadFile(const string& path);
    static bool WriteToFile(const string& path, const string& contents);

};

}

#endif // MORPH_RESOURCE_STORAGE_HPP