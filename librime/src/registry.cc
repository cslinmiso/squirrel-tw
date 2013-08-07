//
// Copyleft 2011 RIME Developers
// License: GPLv3
//
// 2011-03-14 GONG Chen <chen.sst@gmail.com>
//
#include <rime/common.h>
#include <rime/component.h>
#include <rime/registry.h>

namespace rime {

scoped_ptr<Registry> Registry::instance_;

void Registry::Register(const std::string &name, ComponentBase *component) {
  LOG(INFO) << "registering component: " << name;
  map_[name] = component;
}

void Registry::Unregister(const std::string &name) {
  LOG(INFO) << "unregistering component: " << name;
  ComponentMap::iterator it = map_.find(name);
  if (it == map_.end())
    return;
  delete it->second;
  map_.erase(it);
}

void Registry::Clear() {
  ComponentMap::iterator it = map_.begin();
  while (it != map_.end()) {
    delete it->second;
    map_.erase(it++);
  }
}

ComponentBase* Registry::Find(const std::string &name) {
  ComponentMap::const_iterator it = map_.find(name);
  if (it != map_.end()) {
    return it->second;
  }
  return NULL;
}

}  // namespace rime
