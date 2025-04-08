"use client";
import Footer from "@/components/footer";
import Header from "../components/header";
import { Input } from "@/components/ui/input";
import { useEffect, useState } from "react";
import { Button } from "@/components/ui/button";
import { useSearchParams } from "next/navigation";
import { useRouter } from "next/navigation";
import { useTheme } from "next-themes";
import Image from "next/image";
import Link from "next/link";
import LightLogo from "@/components/light-logo.png";
import DarkLogo from "@/components/dark-logo.png";
import {
  Card,
  CardContent,
  CardDescription,
  CardTitle,
} from "@/components/ui/card";
import RenderPaginationItems from "@/components/pagination-pages";
import {
  Pagination,
  PaginationContent,
  PaginationItem,
  PaginationNext,
  PaginationPrevious,
} from "@/components/ui/pagination";

const HomePage: React.FC = () => {
  const searchParams = useSearchParams();
  const router = useRouter();
  const { theme } = useTheme();

  const [logoSrc, setLogoSrc] = useState(LightLogo);

  const [searchQuery, setSearchQuery] = useState<string | null>(
    searchParams.get("query"),
  );

  const [showResults, setShowResults] = useState<boolean>(false);

  const [results, setResults] = useState<SearchResults[]>([]);

  // determines whether the results are being loaded
  const [loadingState, setLoadingState] = useState<boolean>(false);

  // NOTE: Used to test whether searchQuery is correct
  // biome-ignore lint/correctness/useExhaustiveDependencies: We don't worry about setShowResults
  useEffect(() => {
    setLogoSrc(theme === "dark" ? DarkLogo : LightLogo);
    if (searchParams.get("query")) {
      setShowResults(true);
      setPageAndResults();
    } else {
      setShowResults(false);
    }
  }, [searchParams, searchQuery, theme]);

  // NOTE: Used to call backend API to fetch the results
  const onSubmit = () => {
    if (searchQuery) {
      addQueryParam(searchQuery);
      setShowResults(true);
      setPageAndResults();
      fetchData();
    }
  };

  const handleKeyDown = (event: React.KeyboardEvent) => {
    if (event.key === "Enter") {
      onSubmit();
    }
  };

  function sleep(ms: number) {
    return new Promise((resolve) => setTimeout(resolve, ms));
  }

  const setPageAndResults = async () => {
    setLoadingState(true);
    await sleep(1 * 1000);
    // TODO: Set the fetched results here ...
    setResults(results);
    setTotalPages(Math.ceil(results.length / itemsPerPage)); // set the total pages that we can show (+1 so we don't lose results to show)
    setLoadingState(false);

    const pageIndex = Number(searchParams.get("pages"));
    if (pageIndex) {
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
    return pageCount || 1;
  });
  const maxPagesToShow = 3;
  const itemsPerPage = 15;

  const currentPagesResults = results.slice(
    (currentPage - 1) * itemsPerPage,
    currentPage * itemsPerPage,
  );

  const fetchData = async () => {
    const url = `/api/search?query=${searchQuery}`;

    return await fetch(url, { method: "GET" })
      .then((response) => {
        if (response.ok) {
          return response.json(); // Note the parentheses here
        }
        throw new Error("Failed to fetch data");
      })
      .then((data) => {
        console.log(data.docs);
        setResults(data.docs);
      })
      .catch((error) => {
        console.log(error);
        console.error(error);
      });
  };

  return (
    <div>
      {/* Show the search results  */}
      {!loadingState ? (
        showResults ? (
          // need to update header to be sticky
          <div>
            <div className="sticky top-0">
              <Header
                searchQuery={searchQuery || ""}
                setSearchQuery={setSearchQuery}
                onSubmit={onSubmit}
                minimal={false}
              />
            </div>

            <div className="mt-20">
              {/* Set loading state to true */}
              {currentPagesResults.map((result) => (
                <div key={result.id} className="px-10 py-5">
                  <Link href={result.url}>
                    <Card className="p-5 shadow-lg">
                      <CardContent>
                        <CardTitle>{result.url}</CardTitle>
                        {result.title}
                        <CardDescription>{result.snippet}</CardDescription>
                      </CardContent>
                    </Card>
                  </Link>
                </div>
              ))}
              {/* Set loading state to false */}
            </div>
            {/* Pagination */}
            {totalPages > 1 && (
              // need to update to be sticky on bottom
              <Pagination className="fixed bottom-28">
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
                        currentPage < totalPages &&
                          addPageParam(currentPage + 1);
                      }}
                    />
                  </PaginationItem>
                </PaginationContent>
              </Pagination>
            )}
          </div>
        ) : (
          <div>
            {/*No search results (i.e. did not click the search button, or does not have any search params in the URL)*/}
            <div className="sticky">
              <Header
                searchQuery={""}
                setSearchQuery={setSearchQuery}
                onSubmit={onSubmit}
                minimal={true}
              />
            </div>

            <div className="min-h-screen w-full flex flex-col justify-center items-center space-y-5">
              <Image
                src={logoSrc}
                alt="SearchRPI Logo"
                style={{ width: "600px", height: "300px", objectFit: "cover" }}
              />
              <Input
                value={searchQuery || ""}
                onChange={(e) => setSearchQuery(e.target.value)}
                onKeyDown={handleKeyDown}
                className="w-[40vw]"
                placeholder="Search here ..."
                id="SearchBox"
              />
              <Button className="mt-3" onClick={onSubmit}>
                Search
              </Button>
            </div>
          </div>
        )
      ) : (
        <div className="flex justify-center items-center h-[50vh] ">
          <div
            className="inline-block h-8 w-8 animate-spin rounded-full border-4 border-solid border-current border-e-transparent align-[-0.125em] text-surface motion-reduce:animate-[spin_1.5s_linear_infinite] dark:text-white"
            // biome-ignore lint/a11y/useSemanticElements: it works
            role="status"
          >
            <span className="!absolute !-m-px !h-px !w-px !overflow-hidden !whitespace-nowrap !border-0 !p-0 ![clip:rect(0,0,0,0)]">
              Loading...
            </span>
          </div>
        </div>
      )}
      <div className="fixed bottom-5 left-1/2 transform -translate-x-1/2">
        <Footer />
      </div>
    </div>
  );
};

export default HomePage;
