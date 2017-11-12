#ifndef NANO_ENGINE_SYSTEM_HPP_
#define NANO_ENGINE_SYSTEM_HPP_

namespace ne
{
class engine;

class system
{
public:
  virtual ~system() = default;

  virtual void initialize() { }
  virtual void tick      () { }
  virtual void terminate () { }

protected:
  friend class engine;

  engine* engine_ = nullptr;
};
}

#endif
