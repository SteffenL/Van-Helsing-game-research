#include <common/Backup.h>
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <cassert>

namespace common {

Backup::Backup(const std::string& backupDir) : m_backupDir(backupDir) {}

// Format: yyyymmddhhmmss
const char BackupFolderNameDateFormat[] = "%Y%m%d%H%M%S";

std::string Backup::GetBackupRootPath(const std::string& fullPath)
{
    boost::filesystem::path backupRootPath(fullPath);
    const std::string& fileName = backupRootPath.filename().string();
    backupRootPath.remove_filename();
    backupRootPath /= boost::filesystem::path(m_backupDir);
    backupRootPath /= fileName;
    return backupRootPath.parent_path().string();
}
/*

wxString GetBackupPath(const wxString& fullPath)
{
    wxFileName tempPath(GetBackupRootPath(fullPath), L"");
    tempPath.AppendDir(wxFileName(fullPath).GetFullName());
    return tempPath.GetPath();
}

bool BackupFile(const wxString& fullPath)
{
    using namespace config;

    wxString backupPath(GetBackupPath(fullPath));
    wxString backupFolderName(wxDateTime::Now().Format(BackupFolderNameDateFormat));
    wxString backupFileName(wxFileName(fullPath).GetFullName());

    // Create backup folder
    wxFileName backupFullPath(backupPath, L"");
    backupFullPath.AppendDir(backupFolderName);

    // If folder exists, make a new name
    if (backupFullPath.DirExists())
    {
        unsigned int counter = 1;

        do
        {
            wxString newFolderName(wxString::Format(L"%s-%d", backupFolderName, counter));

            backupFullPath.Clear();
            backupFullPath.AssignDir(backupPath);
            backupFullPath.AppendDir(newFolderName);

            ++counter;

            // Stop after trying too hard
            if (counter >= 100)
            {
                // TODO: Handle error
                return false;
            }
        } while (backupFullPath.DirExists());
    }

    backupFullPath.SetFullName(backupFileName);

#if 0
    // A backup with this name exists, make a new name
    // This may happen if the user repeatedly saves quickly
    if (backupFullPath.FileExists())
    {
        unsigned int counter = 1;
        wxFileName tempFullPath(backupFullPath);

        do
        {
            wxString extChar = (tempFullPath.HasExt() ? L"." : wxEmptyString);
            tempFullPath.SetName(wxString::Format(L"%s-%d%s%s", tempFullPath.GetName(), counter, extChar, tempFullPath.GetExt()));
            ++counter;

            // Stop after trying too hard
            if (counter >= 100)
            {
                // TODO: Handle error
                return false;
            }
        } while (tempFullPath.FileExists());

        backupFullPath = tempFullPath;
    }
#endif

    // Create backup folder
    if (!wxFileName::Mkdir(backupFullPath.GetPath(), 0700, wxPATH_MKDIR_FULL))
    {
        // TODO: Handle error
        return false;
    }

    // Move original file
    if (!wxRenameFile(fullPath, backupFullPath.GetFullPath()))
    {
        // TODO: Handle error
        return false;
    }

    // Copy to the original location
    if (!wxCopyFile(backupFullPath.GetFullPath(), fullPath))
    {
        // TODO: Handle error
        return false;
    }

    return true;
}

int GetBackupsRemaining(const wxString& fullPath)
{
    using namespace config;

    int maxBackupCount = ConfigStuff::GetPrefInt32(pref::Backups_MaxCopies);
    int currentBackupCount = 0;

    wxString backupPath(GetBackupPath(fullPath));
    wxDir dir(backupPath);

    if (!wxDirExists(backupPath))
    {
        return maxBackupCount;
    }

    wxString fileName;
    bool found = dir.GetFirst(&fileName);
    while (found)
    {
        ++currentBackupCount;
        // Stop when max is reached
        if (currentBackupCount >= maxBackupCount)
        {
            break;
        }

        found = dir.GetNext(&fileName);
    }

    int backupsRemaining = (maxBackupCount - currentBackupCount);
    return backupsRemaining;
}

bool DeleteOlderBackups(const wxString& fullPath)
{
    using namespace config;

    unsigned int maxBackupCount = ConfigStuff::GetPrefInt32(pref::Backups_MaxCopies);
    unsigned int currentBackupCount = 0;

    wxString backupPath(GetBackupPath(fullPath));
    wxDir dir(backupPath);

    if (!wxDirExists(backupPath))
    {
        return true;
    }

    // Count / Find backups
    wxArrayString backupFolders;
    wxString fileName;
    bool dirFound = dir.GetFirst(&fileName);
    while (dirFound)
    {
        ++currentBackupCount;
        backupFolders.Add(fileName);
        dirFound = dir.GetNext(&fileName);
    }

    // Sort (from old to new)
    backupFolders.Sort();

    if (backupFolders.GetCount() < maxBackupCount)
    {
        return true;
    }

    // How many do we need to delete? Make room for the new backup
    unsigned int deleteCount = (backupFolders.GetCount() - maxBackupCount) + 1;

    // Delete
    // Maximum error count = 10% of max backup count, round down
    // TODO: Make max error count user-configurable
    const unsigned int MaxErrorCount = floor((maxBackupCount / 100.0) * 0.0);
    unsigned int i = 0;
    unsigned int errorCount = 0;
    for (wxArrayString::iterator it = backupFolders.begin(), end_ = backupFolders.end(); (it != end_), (i < deleteCount); ++it, ++i)
    {
        wxFileName tempBackupFullPath(backupPath, L"");
        tempBackupFullPath.AppendDir(*it);

        if (!wxFileName::Rmdir(tempBackupFullPath.GetPath(), wxPATH_RMDIR_RECURSIVE))
        {
            ++errorCount;
        }

        if (errorCount > MaxErrorCount)
        {
            // TODO: Handle error
            return false;
        }
    }

    return true;
}*/

}
