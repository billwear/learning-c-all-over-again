# Calendar Utility Functional Specification

This spec dictates exactly how the calendar parsing engine must behave.

## 1. Stream Format
* File location must live at `~/.calendar`.
* Valid lines require a date token, a literal tab character (`\t`), and a description.
* Anything else gets silently dropped.

## 2. The Token Matching Rules
Assuming today is **June 18, 2026**:

| Format | Example | My Evaluation Rule | Does it Match Today? |
| :--- | :--- | :--- | :--- |
| **Absolute Date** | `06/18/2026` | Matches month, day, and year exactly. | **MATCH** |
| **Yearly Recurring** | `06/18` | Matches month and day; skips the year check. | **MATCH** |
| **Monthly Wildcard** | `06 *` | Matches if the number matches the current month and has a trailing `*`. | **MATCH** (All June events) |
| **DOM Recurring** | `18` | Matches if a single number with no `*` matches the current day of the month. | **MATCH** (Every 18th) |
| **Mismatch** | `06/18/2024` | Fails because the explicit year parameter is in the past. | **TODO** |
| **Daily Reminder** | `daily` | matches every day of the year | ** TODO ** |
| **Lowercase comparison** | all alpha strings | alpha is converted to lc before comparison |
| **Weekend Reminders** | `weekend` | matches saturday or sunday | **TODO** |
| **Weekday Reminder** | `weekday` | matches mon-fri | **TODO** |
| **Day of Week** | e.g., `fri` | matches specified day of the week | **TODO** |
