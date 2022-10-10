#pragma once

#include <Core/Joins.h>

#include <Storages/IStorage_fwd.h>
#include <Storages/TableLockHolder.h>
#include <Storages/StorageSnapshot.h>

#include <Interpreters/Context_fwd.h>
#include <Interpreters/StorageID.h>

#include <Analyzer/IQueryTreeNode.h>

namespace DB
{

/** Join node represents join in query tree.
  *
  * For JOIN without join expression, JOIN expression is null.
  * Example: SELECT id FROM test_table_1 AS t1, test_table_2 AS t2;
  *
  * For JOIN with USING, JOIN expression contains list of identifier nodes. These nodes must be resolved
  * during query analysis pass.
  * Example: SELECT id FROM test_table_1 AS t1 INNER JOIN test_table_2 AS t2 USING (id);
  *
  * For JOIN with ON, JOIN expression contains single expression.
  * Example: SELECT id FROM test_table_1 AS t1 INNER JOIN test_table_2 AS t2 ON t1.id = t1.id;
  */
class JoinNode;
using JoinNodePtr = std::shared_ptr<JoinNode>;

class JoinNode final : public IQueryTreeNode
{
public:
    /** Construct join node with left table expression, right table expression and join expression.
      * Example: SELECT id FROM test_table_1 INNER JOIN test_table_2 ON expression.
      *
      * test_table_1 - left table expression.
      * test_table_2 - right table expression.
      * join_expression - join_expression;
      */
    JoinNode(QueryTreeNodePtr left_table_expression_,
        QueryTreeNodePtr right_table_expression_,
        QueryTreeNodePtr join_expression_,
        JoinLocality locality_,
        JoinStrictness strictness_,
        JoinKind kind_);

    /** Construct join node with left table expression, right table expression and using identifiers.
      * Example: SELECT id FROM test_table_1 INNER JOIN test_table_2 USING (using_identifier, ...).
      * test_table_1 - left table expression.
      * test_table_2 - right table expression.
      * (using_identifier, ...) - using identifiers.
      */
    JoinNode(QueryTreeNodePtr left_table_expression_,
        QueryTreeNodePtr right_table_expression_,
        QueryTreeNodes using_identifiers,
        JoinLocality locality_,
        JoinStrictness strictness_,
        JoinKind kind_);

    /// Get left table expression
    const QueryTreeNodePtr & getLeftTableExpression() const
    {
        return children[left_table_expression_child_index];
    }

    /// Get left table expression
    QueryTreeNodePtr & getLeftTableExpression()
    {
        return children[left_table_expression_child_index];
    }

    /// Get right table expression
    const QueryTreeNodePtr & getRightTableExpression() const
    {
        return children[right_table_expression_child_index];
    }

    /// Get right table expression
    QueryTreeNodePtr & getRightTableExpression()
    {
        return children[right_table_expression_child_index];
    }

    /// Get join expression
    const QueryTreeNodePtr & getJoinExpression() const
    {
        return children[join_expression_child_index];
    }

    /// Get join expression
    QueryTreeNodePtr & getJoinExpression()
    {
        return children[join_expression_child_index];
    }

    bool isUsingJoinExpression() const
    {
        return getJoinExpression() && getJoinExpression()->getNodeType() == QueryTreeNodeType::LIST;
    }

    bool isOnJoinExpression() const
    {
        return getJoinExpression() && getJoinExpression()->getNodeType() != QueryTreeNodeType::LIST;
    }

    JoinLocality getLocality() const
    {
        return locality;
    }

    JoinStrictness getStrictness() const
    {
        return strictness;
    }

    JoinKind getKind() const
    {
        return kind;
    }

    ASTPtr toASTTableJoin() const;

    QueryTreeNodeType getNodeType() const override
    {
        return QueryTreeNodeType::JOIN;
    }

    void dumpTreeImpl(WriteBuffer & buffer, FormatState & format_state, size_t indent) const override;

protected:
    bool isEqualImpl(const IQueryTreeNode & rhs) const override;

    void updateTreeHashImpl(HashState & state) const override;

    QueryTreeNodePtr cloneImpl() const override;

    ASTPtr toASTImpl() const override;

private:
    JoinLocality locality = JoinLocality::Unspecified;
    JoinStrictness strictness = JoinStrictness::Unspecified;
    JoinKind kind = JoinKind::Inner;

    static constexpr size_t left_table_expression_child_index = 0;
    static constexpr size_t right_table_expression_child_index = 1;
    static constexpr size_t join_expression_child_index = 2;
    static constexpr size_t children_size = join_expression_child_index + 1;
};

}

