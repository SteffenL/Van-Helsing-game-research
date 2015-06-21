#ifndef steffenl_common_backup_
#define steffenl_common_backup_

#include <string>

namespace steffenl { namespace common {

class Backup
{
public:
    Backup(const std::string& backupDir);
    std::string GetBackupRootPath(const std::string& fullPath);
    /*std::string GetBackupPath(const std::string& fullPath);
    bool BackupFile(const std::string& filePath);
    int GetBackupsRemaining(const std::string& fullPath);
    bool DeleteOlderBackups(const std::string& filePath);*/

private:
    std::string m_backupDir;
};

}} // namespace
#endif // guard