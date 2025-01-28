import { useEffect, useState } from "react";
import {
  PaginationEllipsis,
  PaginationItem,
  PaginationLink,
} from "./ui/pagination";

interface Props {
  totalPages: number;
  maxPagesToShow: number;
  currentPage: number;
  setCurrentPage: React.Dispatch<React.SetStateAction<number>>;
}

const RenderPaginationItems: React.FC<Props> = ({
  totalPages,
  maxPagesToShow,
  currentPage,
  setCurrentPage,
}) => {
  /**
   * Generates a random id/key for a unique key for lists
   *
   * @param {number} length - The length of the key that is going to be generated
   */
  const makeid = (length: number) => {
    let result = "";
    const characters =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const charactersLength = characters.length;
    let counter = 0;
    while (counter < length) {
      result += characters.charAt(Math.floor(Math.random() * charactersLength));
      counter += 1;
    }
    return result;
  };
  // reset the pagination list
  // biome-ignore lint/correctness/useExhaustiveDependencies: its fine
  useEffect(() => {
    const currentPagination = [];
    if (totalPages > maxPagesToShow) {
      let max = 1;
      let min = 1;
      // Find the upper bound (the highest page to show)
      max =
        currentPage === 1
          ? Math.min(totalPages, currentPage + Math.ceil(maxPagesToShow / 2))
          : Math.min(totalPages, currentPage + Math.floor(maxPagesToShow / 2));
      // Find the lower bound (the lowest page to show)
      min =
        currentPage === totalPages
          ? Math.max(1, currentPage - Math.ceil(maxPagesToShow / 2))
          : Math.max(1, currentPage - Math.floor(maxPagesToShow / 2));

      // Show first page if current page is far away
      if (currentPage - maxPagesToShow > 1) {
        currentPagination.push(
          <PaginationItem key={makeid(5)}>
            <PaginationLink
              className={1 === currentPage ? "bg-gray-300" : ""}
              onClick={() => setCurrentPage(1)}
            >
              1
            </PaginationLink>
          </PaginationItem>,
        );
      }

      // Add ellipsis to beginning if needed
      if (min !== 1) {
        currentPagination.push(
          <PaginationItem key={makeid(5)}>
            <PaginationEllipsis />
          </PaginationItem>,
        );
      }

      // Add the page pagination items for the pages we want to view
      for (let i = min; i <= max; i++) {
        currentPagination.push(
          <PaginationItem key={makeid(5)}>
            <PaginationLink
              className={i === currentPage ? "bg-gray-300" : ""}
              onClick={() => setCurrentPage(i)}
            >
              {i}
            </PaginationLink>
          </PaginationItem>,
        );
      }

      // Add ellipsis to end if needed
      if (max !== totalPages) {
        currentPagination.push(
          <PaginationItem key={makeid(5)}>
            <PaginationEllipsis />
          </PaginationItem>,
        );
      }
      // Show last page if current page is far away
      if (currentPage + maxPagesToShow < totalPages) {
        currentPagination.push(
          <PaginationItem key={makeid(5)}>
            <PaginationLink
              className={totalPages === currentPage ? "bg-gray-300" : ""}
              onClick={() => setCurrentPage(totalPages)}
            >
              {totalPages}
            </PaginationLink>
          </PaginationItem>,
        );
      }
    } else {
      // Show all pages if there are fewer than maxPagesToShow
      for (let i = 1; i <= totalPages; i++) {
        currentPagination.push(
          <PaginationItem key={makeid(5)}>
            <PaginationLink
              className={i === currentPage ? "bg-gray-300" : ""}
              onClick={() => setCurrentPage(i)}
            >
              {i}
            </PaginationLink>
          </PaginationItem>,
        );
      }
    }
    setPagination(currentPagination);
  }, [currentPage, totalPages, setCurrentPage]);

  const [pagination, setPagination] = useState<React.ReactNode[]>([]);

  return <div className="flex">{pagination}</div>;
};

export default RenderPaginationItems;
