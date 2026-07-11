#pragma once
#include <vector>
#include <memory>
#include <functional>

struct SoldatBlackboard;

enum class BTStatus { Success, Failure, Running };
struct SoldatBlackboard;

class BTNode {
public:
    virtual ~BTNode() = default;
    virtual BTStatus Tick(SoldatBlackboard& bb) = 0;
};
using BTNodePtr = std::unique_ptr<BTNode>;

class BTSelector : public BTNode {
public:
    std::vector<BTNodePtr> children;
    BTStatus Tick(SoldatBlackboard& bb) override {
        for (auto& c : children) {
            BTStatus s = c->Tick(bb);
            if (s != BTStatus::Failure) return s;
        }
        return BTStatus::Failure;
    }
};

class BTSequence : public BTNode {
public:
    std::vector<BTNodePtr> children;
    BTStatus Tick(SoldatBlackboard& bb) override {
        for (auto& c : children) {
            BTStatus s = c->Tick(bb);
            if (s != BTStatus::Success) return s;
        }
        return BTStatus::Success;
    }
};

class BTCondition : public BTNode {
    std::function<bool(SoldatBlackboard&)> predicate;
public:
    explicit BTCondition(std::function<bool(SoldatBlackboard&)> p) : predicate(std::move(p)) {}
    BTStatus Tick(SoldatBlackboard& bb) override {
        return predicate(bb) ? BTStatus::Success : BTStatus::Failure;
    }
};

class BTAction : public BTNode {
    std::function<BTStatus(SoldatBlackboard&)> action;
public:
    explicit BTAction(std::function<BTStatus(SoldatBlackboard&)> a) : action(std::move(a)) {}
    BTStatus Tick(SoldatBlackboard& bb) override {
        return action(bb);
    }
};

template <typename... Args>
BTNodePtr MakeSelector(Args... args) {
    auto n = std::make_unique<BTSelector>();
    (n->children.push_back(std::move(args)), ...);
    return n;
}
template <typename... Args>
BTNodePtr MakeSequence(Args... args) {
    auto n = std::make_unique<BTSequence>();
    (n->children.push_back(std::move(args)), ...);
    return n;
}
inline BTNodePtr MakeCondition(std::function<bool(SoldatBlackboard&)> p) {
    return std::make_unique<BTCondition>(std::move(p));
}
inline BTNodePtr MakeAction(std::function<BTStatus(SoldatBlackboard&)> a) {
    return std::make_unique<BTAction>(std::move(a));
}