"use client";
import Footer from "@/components/footer";
import Header from "../components/header";
import { Input } from "@/components/ui/input";
import { useEffect, useState } from "react";
import { Button } from "@/components/ui/button";

const HomePage: React.FC = () => {
  const [searchQuery, setSearchQuery] = useState<string>("");

  // NOTE: Used to test whether searchQuery is correct
  useEffect(() => {
    console.log(searchQuery);
  }, [searchQuery]);

  // NOTE: Used to call backend API to fetch the results
  const onSubmit = () => {
    console.log("Search button clicked");
    console.log(`Search Query: ${searchQuery}`);
  };

  return (
    <div>
      <Header />
      <div className="h-[80vh] mt-20">
        <div className="flex justify-center">
          {/* TODO: Handle "Submit" or "Enter" key press inside of the Input */}
          <Input
            value={searchQuery}
            onChange={(e) => setSearchQuery(e.target.value)}
            className="w-[50vw]"
            placeholder="Search here ..."
          />
          <Button onClick={onSubmit}>Search</Button>
        </div>
      </div>

      <Footer />
    </div>
  );
};

export default HomePage;
