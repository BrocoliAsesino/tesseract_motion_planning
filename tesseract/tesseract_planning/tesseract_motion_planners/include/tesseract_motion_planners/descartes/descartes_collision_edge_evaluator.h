/**
 * @file descartes_collision_edge_evaluator.h
 * @brief Tesseract Descartes Collision Edge Evaluator Implementation
 *
 * @author Levi Armstrong
 * @date December 18, 2019
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2019, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TESSERACT_MOTION_PLANNERS_DESCARTES_COLLISION_EDGE_EVALUATOR_H
#define TESSERACT_MOTION_PLANNERS_DESCARTES_COLLISION_EDGE_EVALUATOR_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <memory>
#include <vector>
#include <descartes_light/interface/edge_evaluator.h>
#include <tesseract_environment/core/environment.h>
#include <tesseract_collision/core/discrete_contact_manager.h>
#include <tesseract_collision/core/continuous_contact_manager.h>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

namespace tesseract_motion_planners
{
template <typename FloatType>
class DescartesCollisionEdgeEvaluator : public descartes_light::EdgeEvaluator<FloatType>
{
public:
  DescartesCollisionEdgeEvaluator(const tesseract_environment::Environment::ConstPtr& collision_env,
                                  std::vector<std::string> active_links,
                                  std::vector<std::string> joint_names,
                                  double collision_safety_margin = 0.025,
                                  double longest_valid_segment_length = 0.5,
                                  bool allow_collision = false,
                                  bool debug = false);

  bool evaluate(const descartes_light::Rung_<FloatType>& from,
                const descartes_light::Rung_<FloatType>& to,
                std::vector<typename descartes_light::LadderGraph<FloatType>::EdgeList>& edges) override;

protected:
  tesseract_environment::StateSolver::Ptr state_solver_; /**< @brief The tesseract state solver */
  tesseract_scene_graph::AllowedCollisionMatrix acm_;    /**< @brief The allowed collision matrix */
  std::vector<std::string> active_link_names_;           /**< @brief A vector of active link names */
  std::vector<std::string> joint_names_;                 /**< @brief A vector of joint names */
  tesseract_collision::DiscreteContactManager::Ptr discrete_contact_manager_; /**< @brief The discrete contact manager
                                                                               */
  tesseract_collision::ContinuousContactManager::Ptr continuous_contact_manager_; /**< @brief The discrete contact
                                                                                     manager */
  double collision_safety_margin_;      /**< @brief The minimum allowed collision distance */
  double longest_valid_segment_length_; /**< @brief Used to check collisions between two state if norm(state0-state1) >
                                           longest_valid_segment_length. */
  bool allow_collision_; /**< @brief If true and no valid edges are found it will return the one with the lowest cost */
  bool debug_;           /**< @brief Enable debug information to be printed to the terminal */
  std::size_t dof_;      /**< @brief The number of joints */

  /**
   * @brief Check continuous and discrete collision between two states
   * @param out Output edge lists
   * @param start Start state
   * @param end End state
   * @param next_idx Next idex
   * @param find_best Indicate if best solution should be found
   */
  void considerEdge(typename descartes_light::LadderGraph<FloatType>::EdgeList& out,
                    const FloatType* start,
                    const FloatType* end,
                    std::size_t next_idx,
                    bool find_best);

  /**
   * @brief Check if two links are allowed to be in collision
   * @param a The name of the first link
   * @param b The name of the second link
   * @return True if allowed to be in collision, otherwise false
   */
  bool isContactAllowed(const std::string& a, const std::string& b) const;
};

using DescartesCollisionEdgeEvaluatorF = DescartesCollisionEdgeEvaluator<float>;
using DescartesCollisionEdgeEvaluatorD = DescartesCollisionEdgeEvaluator<double>;

}  // namespace tesseract_motion_planners
#endif  // TESSERACT_MOTION_PLANNERS_DESCARTES_COLLISION_EDGE_EVALUATOR_H