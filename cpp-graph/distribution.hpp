#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP

#include <functional>
#include <map>
#include <vector>

#include "../stats/include/stats.hpp"

class Distribution {
 public:
  Distribution() = default;

  Distribution(const Distribution&) = default;
  Distribution(Distribution&&) = default;

  Distribution& operator=(const Distribution&) = default;
  Distribution& operator=(Distribution&&) = default;

  virtual ~Distribution() = default;

  double operator()(size_t degree) const {
    if (probabilities_.find(degree) == probabilities_.end()) {
      return 0.0;
    }

    return probabilities_.at(degree);
  }

  virtual void update(const std::vector<size_t>& degrees) = 0;

 protected:
  std::map<size_t, double> probabilities_;
};

class ConstantDistribution : public Distribution {
 public:
  ConstantDistribution(double probability) : probability_func_([probability](size_t) { return probability; }) {}
  ConstantDistribution(std::function<double(size_t)> func) : probability_func_(func) {}

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();
    for (const auto degree : degrees) {
      probabilities_[degree] = probability_func_(degree);
    }
  }

 private:
  std::function<double(size_t)> probability_func_;
};

class UniformDistribution : public Distribution {
 public:
  UniformDistribution() = default;

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();
    for (const auto degree : degrees) {
      probabilities_[degree] = 1.0 / degrees.size();
    }
  }
};

class ProportionalDistribution : public Distribution {
 public:
  ProportionalDistribution() = default;

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();

    double sum = 0.0;
    for (const auto degree : degrees) {
      probabilities_[degree] = static_cast<double>(degree);
      sum += probabilities_[degree];
    }

    for (auto& prob : probabilities_) {
      prob.second /= sum;
    }
  }
};

class PowerDistribution : public Distribution {
 public:
  PowerDistribution(double exponent) : exponent_(exponent) {}

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();

    double sum = 0.0;
    for (const auto degree : degrees) {
      probabilities_[degree] = std::pow(degree, exponent_);
      sum += probabilities_[degree];
    }

    for (auto& prob : probabilities_) {
      prob.second /= sum;
    }
  }

 private:
  double exponent_;
};

class NormalDistribution : public Distribution {
 public:
  NormalDistribution(double mean, double stddev) : mean_(mean), stddev_(stddev) {}

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();

    double sum = 0.0;
    for (const auto degree : degrees) {
      probabilities_[degree] = stats::dnorm(degree, mean_, stddev_);
      sum += probabilities_[degree];
    }

    for (auto& prob : probabilities_) {
      prob.second /= sum;
    }
  }

 private:
  double mean_;
  double stddev_;
};

class LogNormalDistribution : public Distribution {
 public:
  LogNormalDistribution(double mean, double stddev) : mean_(mean), stddev_(stddev) {}

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();

    double sum = 0.0;
    for (const auto degree : degrees) {
      probabilities_[degree] = stats::dlnorm(degree, mean_, stddev_);
      sum += probabilities_[degree];
    }

    for (auto& prob : probabilities_) {
      prob.second /= sum;
    }
  }

 private:
  double mean_;
  double stddev_;
};

class ExponentialDistribution : public Distribution {
 public:
  ExponentialDistribution(double lambda) : lambda_(lambda) {}

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();

    double sum = 0.0;
    for (const auto degree : degrees) {
      probabilities_[degree] = stats::dexp(degree, lambda_);
      sum += probabilities_[degree];
    }

    for (auto& prob : probabilities_) {
      prob.second /= sum;
    }
  }

 private:
  double lambda_;
};

class LogDistribution : public Distribution {
 public:
  LogDistribution(double base) : base_(base) {}

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();

    double sum = 0.0;
    for (const auto degree : degrees) {
      probabilities_[degree] = std::log(degree + 1) / std::log(base_);
      sum += probabilities_[degree];
    }

    for (auto& prob : probabilities_) {
      prob.second /= sum;
    }
  }

 private:
  double base_;
};

class WeibullDistribution : public Distribution {
 public:
  WeibullDistribution(double shape, double scale) : shape_(shape), scale_(scale) {}

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();

    double sum = 0.0;
    for (const auto degree : degrees) {
      probabilities_[degree] = stats::dweibull(degree, shape_, scale_);
      sum += probabilities_[degree];
    }

    for (auto& prob : probabilities_) {
      prob.second /= sum;
    }
  }

 private:
  double shape_;
  double scale_;
};

class GammaDistribution : public Distribution {
 public:
  GammaDistribution(double shape, double scale) : shape_(shape), scale_(scale) {}

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();

    double sum = 0.0;
    for (const auto degree : degrees) {
      probabilities_[degree] = stats::dgamma(degree, shape_, scale_);
      sum += probabilities_[degree];
    }

    for (auto& prob : probabilities_) {
      prob.second /= sum;
    }
  }

 private:
  double shape_;
  double scale_;
};

class ChiSquaredDistribution : public Distribution {
 public:
  ChiSquaredDistribution(double freedom_degrees) : freedom_degrees_(freedom_degrees) {}

  void update(const std::vector<size_t>& degrees) override {
    probabilities_.clear();

    double sum = 0.0;
    for (const auto degree : degrees) {
      probabilities_[degree] = stats::dchisq(degree, freedom_degrees_);
      sum += probabilities_[degree];
    }

    for (auto& prob : probabilities_) {
      prob.second /= sum;
    }
  }

 private:
  double freedom_degrees_;
};

#endif  // DISTRIBUTION_HPP