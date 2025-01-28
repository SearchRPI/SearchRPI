"use client";
import Footer from "@/components/footer";
import Header from "../components/header";
import { Input } from "@/components/ui/input";
import { useEffect, useState } from "react";
import { Button } from "@/components/ui/button";
import { useSearchParams } from "next/navigation";
import { useRouter } from "next/navigation";
import Link from "next/link";
import { Card, CardContent, CardTitle } from "@/components/ui/card";
import RenderPaginationItems from "@/components/pagination-pages";
import {
  Pagination,
  PaginationContent,
  PaginationItem,
  PaginationNext,
  PaginationPrevious,
} from "@/components/ui/pagination";

// Fake data
const linksMap: Map<string, string> = new Map([
  ["https://www.google.com", "Google"],
  ["https://www.facebook.com", "Facebook"],
  ["https://www.twitter.com", "Twitter"],
  ["https://www.linkedin.com", "LinkedIn"],
  ["https://www.github.com", "GitHub"],
  ["https://www.reddit.com", "Reddit"],
  ["https://www.youtube.com", "YouTube"],
  ["https://www.amazon.com", "Amazon"],
  ["https://www.wikipedia.org", "Wikipedia"],
  ["https://www.stackoverflow.com", "Stack Overflow"],
  ["https://www.apple.com", "Apple"],
  ["https://www.microsoft.com", "Microsoft"],
  ["https://www.instagram.com", "Instagram"],
  ["https://www.pinterest.com", "Pinterest"],
  ["https://www.netflix.com", "Netflix"],
  ["https://www.spotify.com", "Spotify"],
  ["https://www.twitch.tv", "Twitch"],
  ["https://www.bbc.com", "BBC"],
  ["https://www.nytimes.com", "New York Times"],
  ["https://www.cnn.com", "CNN"],
]);

const searchResults: SearchResults[] = Array.from(
  linksMap,
  ([link, title]) => ({ link, title }),
);

const HomePage: React.FC = () => {
  const searchParams = useSearchParams();
  const router = useRouter();

  const [searchQuery, setSearchQuery] = useState<string | null>(
    searchParams.get("query"),
  );

  const [showResults, setShowResults] = useState<boolean>(false);

  const [results, setResults] = useState<SearchResults[]>(searchResults);

  // determines whether the results are being loaded
  const [loadingState, setLoadingState] = useState<boolean>(false);

  // NOTE: Used to test whether searchQuery is correct
  // biome-ignore lint/correctness/useExhaustiveDependencies: We don't worry about setShowResults
  useEffect(() => {
    if (searchParams.get("query")) {
      setShowResults(true);
      setPageAndResults();
    } else {
      setShowResults(false);
    }
  }, [searchParams, searchQuery]);

  // NOTE: Used to call backend API to fetch the results
  const onSubmit = () => {
    if (searchQuery) {
      addQueryParam(searchQuery);
      setShowResults(true);
      setPageAndResults();
    }
  };

  const setPageAndResults = () => {
    setLoadingState(true);
    // TODO: Set the fetched results here ...
    setResults(results);
    setTotalPages(Math.ceil(results.length / itemsPerPage)); // set the total pages that we can show (+1 so we don't lose results to show)
    setLoadingState(false);

    const pageIndex = Number(searchParams.get("pages"));
    if (pageIndex) {
      console.log(pageIndex);
      setCurrentPage(pageIndex);
    }
  };

  // Adds the search query param to the URL
  const addQueryParam = (value: string) => {
    const params = new URLSearchParams(searchParams.toString());
    params.set("query", value);
    router.push(`${window.location.pathname}?${params.toString()}`);
  };

  const addPageParam = (value: number) => {
    const params = new URLSearchParams(searchParams.toString());
    params.set("pages", value.toString());
    router.push(`${window.location.pathname}?${params.toString()}`);
  };

  // Used to keep track of pages
  const [totalPages, setTotalPages] = useState<number>(5);
  const [currentPage, setCurrentPage] = useState<number>(() => {
    const pageCount = Number(searchParams.get("pages"));
    console.log(pageCount);
    return pageCount || 1;
  });
  const maxPagesToShow = 3;
  const itemsPerPage = 15;

  const currentPagesResults = results.slice(
    (currentPage - 1) * itemsPerPage,
    currentPage * itemsPerPage,
  );

  return (
    <div>
      <Header />
      {/* Show the search results  */}
      {showResults ? (
        <div className="mt-20">
          <div className="flex justify-center">
            {/* TODO: Handle "Submit" or "Enter" key press inside of the Input */}
            <Input
              value={searchQuery || ""}
              onChange={(e) => setSearchQuery(e.target.value)}
              className="w-[40vw]"
              placeholder="Search here ..."
            />
          </div>
          <div className="flex justify-center mt-3">
            <Button onClick={onSubmit}>Search</Button>
          </div>

          {/* Set loading state to true */}
          {currentPagesResults.map((result) => (
            <div key={result.title} className="px-10 py-5">
              <Link href={result.link}>
                <Card className="p-5 shadow-lg">
                  <CardContent>
                    <CardTitle>{result.link}</CardTitle>
                    {result.title}
                  </CardContent>
                </Card>
              </Link>
            </div>
          ))}
          {/* Set loading state to false */}
        </div>
      ) : (
        // No search results (i.e. did not click the search button, or does not have any search params in the URL)
        <div className="h-[80vh] mt-20">
          <div className="flex justify-center">
            {/* TODO: Handle "Submit" or "Enter" key press inside of the Input */}
            <Input
              value={searchQuery || ""}
              onChange={(e) => setSearchQuery(e.target.value)}
              className="w-[40vw]"
              placeholder="Search here ..."
            />
          </div>
          <div className="flex justify-center mt-3">
            <Button onClick={onSubmit}>Search</Button>
          </div>
        </div>
      )}

      {/* Pagination */}
      {totalPages > 1 && (
        <Pagination className="mt-5">
          <PaginationContent>
            <PaginationItem>
              <PaginationPrevious
                onClick={() => {
                  currentPage > 1 && setCurrentPage((prev) => prev - 1);
                  currentPage > 1 && addPageParam(currentPage - 1);
                }}
              />
            </PaginationItem>
            <RenderPaginationItems
              totalPages={totalPages}
              maxPagesToShow={maxPagesToShow}
              currentPage={currentPage}
              setCurrentPage={setCurrentPage}
              addPageParam={addPageParam}
            />
            <PaginationItem>
              <PaginationNext
                onClick={() => {
                  currentPage < totalPages &&
                    setCurrentPage((prev) => prev + 1);
                  currentPage < totalPages && addPageParam(currentPage + 1);
                }}
              />
            </PaginationItem>
          </PaginationContent>
        </Pagination>
      )}
      <Footer />
    </div>
  );
};

export default HomePage;
