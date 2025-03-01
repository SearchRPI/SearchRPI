# Optimized Query Representation & Traversal

## 1. Overview

This document outlines the optimal structure for storing and traversing query trees efficiently. Instead of a traditional pointer-based tree, we use a **flat indexed array representation**. This structure ensures:

- **O(1) traversal per step** (no stack or recursion)
- **O(n) build time**
- **Minimal memory overhead** (no explicit pointers)
- **Post-order traversal is implicit**, matching ranking flow

---

## 2. Why Not Use a Traditional Tree?

### Problems with Pointer-Based Trees:
- Require explicit traversal logic (stack or recursion)
- Waste memory storing multiple pointers per node
- Impose unnecessary complexity for a **fixed query**

### The Alternative: **Flat Indexed Representation**
Instead of **explicit tree pointers**, we **store traversal order directly** in an **array**, where:
- **Each node stores its children as offsets** (array indices).
- **Traversal is just iterating through the array**.

---

## 3. Data Structure: Indexed Query Representation

Instead of using a tree, we store queries as **a linear array of nodes** with **child relationships stored implicitly**.

| Index | Node Type    | Value                | Child Start | Child Count |
|-------|------------|---------------------|------------|------------|
| 0     | #combine   | w=1.0               | 1          | 3          |
| 1     | #dirichlet |                      | 2          | 1          |
| 2     | #text      | crime()              | -          | 0          |
| 3     | #od:1      |                      | 4          | 2          |
| 4     | #extents   | organized()          | -          | 0          |
| 5     | #extents   | crime()              | -          | 0          |
| 6     | #uw:8      |                      | 7          | 3          |
| 7     | #extents   | international()      | -          | 0          |
| 8     | #extents   | organized()          | -          | 0          |
| 9     | #extents   | crime()              | -          | 0          |

### How This Works:
- **Traversal is implicit** → Each node knows exactly where to go.
- **Children are stored sequentially** → Post-order evaluation is natural.
- **No extra pointers** → Uses integer indices for navigation.

## 4. Traversal Strategy

### **Post-Order Traversal in an Implicit Structure**
Instead of using recursion or a stack, we **process nodes in order from the array**:

1. `#text:crime()` → Process term score
2. `#dirichlet(#text:crime())` → Apply Dirichlet smoothing
3. `#extents:organized()` → Process position data
4. `#extents:crime()` → Process position data
5. `#od:1(#extents:organized(), #extents:crime())` → Compute ordered proximity
6. `#extents:international()` → Process position data
7. `#extents:organized()` → Process position data
8. `#extents:crime()` → Process position data
9. `#uw:8(#extents:international(), #extents:organized(), #extents:crime())` → Compute unordered window
10. `#combine(...)` → Aggregate scores

This **exactly matches the ranking order** used in Galago and BM25 models.

## 5. Building the Query Representation

### **Steps to Convert a Parsed Query into Indexed Format**
1. **Start with the root node** and assign it `index 0`.
2. **Process children in left-to-right order**, assigning them sequential indices.
3. **Store child relationships implicitly**:
   - Each node stores the index of its **first child** (`Child Start`).
   - Each node records the **number of children** (`Child Count`).
4. **Ensure that nodes are stored in post-order** so traversal aligns with ranking flow.

### **How the Query is Represented in the Index**
Instead of storing nodes with explicit pointers, we store them in a **flat array** where:
- Each node **knows where its children start**.
- **Traversal happens naturally** by iterating over the array.

| Index | Node Type    | Value                | Child Start | Child Count |
|-------|------------|---------------------|------------|------------|
| 0     | #combine   | w=1.0               | 1          | 3          |
| 1     | #dirichlet |                      | 2          | 1          |
| 2     | #text      | crime()              | -          | 0          |
| 3     | #od:1      |                      | 4          | 2          |
| 4     | #extents   | organized()          | -          | 0          |
| 5     | #extents   | crime()              | -          | 0          |
| 6     | #uw:8      |                      | 7          | 3          |
| 7     | #extents   | international()      | -          | 0          |
| 8     | #extents   | organized()          | -          | 0          |
| 9     | #extents   | crime()              | -          | 0          |

### **Why This Structure is Efficient**
- **No need for explicit pointers** → Everything is **indexed**.
- **Traversal follows post-order ranking naturally**.
- **Compact memory usage** compared to tree-based storage.

## 6. Traversing for Ranking

### **Simple Post-Order Processing Loop**
Instead of explicit traversal logic, **just loop through the array**:

1. Start at `index 0`
2. Process children first (using `Child Start` and `Child Count`)
3. When children are done, process parent
4. Repeat until the last index

**This means traversal is just iterating over an array**—**no recursion, no stack**.

## 7. Why This is the Best Approach

✅ **O(1) traversal per step** → No lookups, just follow indices  
✅ **O(n) build time** → Linear pass through query tree  
✅ **Minimal memory overhead** → Only stores fixed-size metadata  
✅ **Traversal naturally follows ranking order**  
✅ **No complex data structures needed**  

### **Comparison vs. Other Structures**

| Structure             | Build Time | Traverse Time | Memory Use | Stack-Free? |
|-----------------------|------------|---------------|------------|------------|
| Pointer-Based Tree    | O(n)       | O(n)          | High (pointers) | No |
| Stack-Based Traversal | O(n)       | O(n)          | Medium | No |
| Flat Indexed Array    | O(n)       | O(1) per step | Low | ✅ Yes |

## 8. Conclusion

- **This structure fully eliminates stacks, recursion, and unnecessary pointer overhead**.
- **Traversal is trivial** → Just iterate over the array.
- **Query ranking follows the optimal order** **without extra work**.
- **Build time remains O(n)** and traversal is **O(1) per step**.
