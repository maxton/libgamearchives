#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <map>

#include "gamearchives/stream/stream.h"

namespace gamearchives {

enum NodeType {
  DIRECTORY = 0,
  FILE = 1
};

typedef union {
  void* pointer;
  uint64_t integer;
} ExtInfo;

class Archive;
class IDirectory;

/**
 * Abstract base class for all elements of a filesystem
 */
class IFSNode {
 public:
  IFSNode(std::string name, NodeType type)
      : type_(type), name_(std::move(name)) {}
  virtual ~IFSNode() = default;
  NodeType type() const { return type_; }
  const std::string& name() const { return name_; }
  virtual IDirectory* GetParent() = 0;
 protected:
  NodeType type_;
  std::string name_;
};

/**
 * Abstract base class for files in an archive.
 */
class IFile : public IFSNode {
 public:
  IFile(const std::string& name,
        uint64_t size,
        bool compressed,
        uint64_t compressed_size)
      : IFSNode(name, FILE),
        size_(size),
        compressed_(compressed),
        compressed_size_(compressed_size) {}
  ~IFile() override = default;
  uint64_t size() const { return size_; }
  bool compressed() const { return compressed_; }
  uint64_t compressed_size() const { return compressed_size_; }
  const std::map<std::string,
                 ExtInfo>& extended_info() const { return extended_info_; }
  virtual std::shared_ptr<Stream> Open() = 0;
 protected:
  uint64_t size_;
  bool compressed_;
  uint64_t compressed_size_;
  std::map<std::string, ExtInfo> extended_info_;
};

/**
 * Abstract base class for directories in an archive.
 */
class IDirectory : public IFSNode {
 public:
  explicit IDirectory(const std::string& name) : IFSNode(name, DIRECTORY) {}
  ~IDirectory() override = default;
  /**
   * Tries to get the named file if it exists in this directory.
   * @param name Name of the file to get
   * @param file [out] Reference to file that will be populated if the file was found
   * @return true on success
   */
  virtual bool GetFile(const std::string& name,
                       std::shared_ptr<IFile>& file) = 0;
  /**
   * Tries to get the named directory if it exists in this directory.
   * @param name Name of the directory to get
   * @param file [out] Reference to directory that will be populated if the directory was found
   * @return true on success
   */
  virtual bool GetDir(const std::string& name,
                      std::shared_ptr<IDirectory>& dir) = 0;
  /**
   * Get a list of all FSNode names in this directory
   * @return list of node names
   */
  virtual const std::vector<std::string> ListNodes() = 0;
  /**
   * TODO: Determine if this is a useful function to have...
   * Get a list of all files in this directory
   * @return list of file objects
   */
  //virtual const std::vector<std::shared_ptr<IFile>> GetFiles() = 0;
};

// Rethink this to determine if we actually want a full virtual filesystem.
//class Vfs {
// public:
//  bool Mount(std::shared_ptr<Archive> archive, std::string path);
//  bool Unmount(std::string root);
//  IFile& GetFileAtPath(std::string path);
//  IDirectory& GetDirectoryAtPath(std::string path);
// protected:
//  std::map<std::string, std::shared_ptr<Archive>> mounted_archives_;
//};

/**
 * Abstract base class that represents a logical archive.
 * Classes that implement this class should also define:
 *  static bool TestFile(std::shared_ptr<IFile>)
 *  static Archive LoadFile(std::shared_ptr<IFile>)
 */
class Archive {
 public:
  virtual const std::string& name() const = 0;
  virtual std::shared_ptr<IDirectory>& root() = 0;
};

//struct ArchiveType {
//  std::string name_;
//  std::vector<std::string> extensions_;
//  std::function<bool(std::shared_ptr<IFile>)> test_file_;
//  std::function<Archive(std::shared_ptr<IFile>)> load_file_;
//};

enum ArchiveTestResult {
  NO = 0,
  MAYBE = 1,
  YES = 2
};

class ArchiveProxyBase {
 public:
  virtual ArchiveTestResult TestFile(std::shared_ptr<IFile>&) = 0;
  virtual std::shared_ptr<Archive> LoadFile(std::shared_ptr<IFile>&) = 0;
  virtual const char* GetName() = 0;
  virtual const std::vector<std::string> GetExtensions() = 0;
};

extern std::vector<ArchiveProxyBase*> archive_types;

template<class T>
class ArchiveProxy : public ArchiveProxyBase {
 public:
  ArchiveProxy() { archive_types.push_back(this); }
  ArchiveTestResult TestFile(std::shared_ptr<IFile>& file) override {
    return T::TestFile(file);
  }
  std::shared_ptr<Archive> LoadFile(std::shared_ptr<IFile>& file) override {
    return T::LoadFile(file);
  }
  const char* GetName() override { return T::type_name(); }
  const std::vector<std::string> GetExtensions() override { return T::extensions(); }
};

}