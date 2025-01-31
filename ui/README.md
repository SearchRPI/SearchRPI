# Search

This is the code for the web application of **SearchRPI**. Currently WIP, but can be ran locally following the steps below.

## Stack

* Typescript
* React
* TailwindCSS
* Next.js

## Features

*  Basic UI features 
    * [X] Search box
    * [X] Pages
    * [X] Show search results
    * Header
        * [X] Home page (logo)
        * [X] Dark/Light mode
    * Footer
        * [X] Source code 
        * [X] Description
    * Filter/Sort options
        * [ ] Filter by certain categories
        * [ ] Sort by most visited
* [ ] Connect with back-end API calls.
* [ ] Filtering/Sorting option

## Run

First install the necessary packages.

```bash
npm install

# or

yarn install

# or

bun install
```

Then run the development server with:

```bash
npm run dev
# or
yarn dev
# or
pnpm dev
# or
bun dev
```

Open [http://localhost:3000](http://localhost:3000) with your browser to see the result.

## URL Parameters 

Currently, there are two URL parameters: pages and query. The pages parameter shows the current page and displays the items that matches with the page. The query parameter shows the searched term inside of the search box. These parameters allow the user to go back to a certain search or a certain page. 
