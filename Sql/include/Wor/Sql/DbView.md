
<h2>DbCellView</h2>

<h3>Presentation</h3>

```c++
{
    .name = "Name";
    .value = "Value";
}
```

---

<h2>DbRowView</h2>

<h3>Presentation</h3>

```c++
std::vector<DbCellView cell>
{
    { "EventId", "0" },
    { "Foo", "Foo value" },
    { "Bar", "Bar value" },
    ...
};
```
EventId must be the first element

---

<h2>DbTableView</h2>

<h3>Presentation</h3>

```c++
std::vector<DbRowView>
{
    { DbRowView1, DbRowView2, ...}
};
```
