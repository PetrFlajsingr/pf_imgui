//
// Created by petr.flajsingr on 2/7/2022.
//

#ifndef PF_IMGUI_RESOURCE_H
#define PF_IMGUI_RESOURCE_H

namespace pf::ui::ig {
class BaseResource {
 public:
  virtual ~BaseResource() = 0;
};

BaseResource::~BaseResource() {}

template<typename T>
class Resource : public BaseResource {
 public:
  [[nodiscard]] virtual const T &get() const = 0;

  virtual ~Resource() = 0;
};

template<typename T>
Resource<T>::~Resource() {}

template<typename T>
class ConstResource : public Resource<T> {
 public:
  ConstResource(T value) : value(value) {}
  ~ConstResource() override = default;
  const T &get() const override { return value; }

 private:
  T value;
};
}  // namespace pf::ui::ig

#endif  //PF_IMGUI_RESOURCE_H
