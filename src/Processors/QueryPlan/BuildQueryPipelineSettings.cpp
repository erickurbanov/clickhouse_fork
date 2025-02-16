#include <Processors/QueryPlan/BuildQueryPipelineSettings.h>
#include <Core/Settings.h>
#include <Interpreters/ExpressionActions.h>
#include <Interpreters/Context.h>

namespace DB
{

namespace Setting
{
    extern const SettingsBool query_plan_merge_filters;
}

BuildQueryPipelineSettings BuildQueryPipelineSettings::fromContext(ContextPtr from)
{
    const auto & query_settings = from->getSettingsRef();
    BuildQueryPipelineSettings settings;
    settings.actions_settings = ExpressionActionsSettings::fromSettings(query_settings, CompileExpressions::yes);
    settings.process_list_element = from->getProcessListElement();
    settings.progress_callback = from->getProgressCallback();

    /// Setting query_plan_merge_filters is enabled by default.
    /// But it can brake short-circuit without splitting filter step into smaller steps.
    /// So, enable and disable this optimizations together.
    settings.enable_multiple_filters_transforms_for_and_chain = query_settings[Setting::query_plan_merge_filters];
    return settings;
}

}
