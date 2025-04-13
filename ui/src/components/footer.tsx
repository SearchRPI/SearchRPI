import { GithubIcon } from "lucide-react";
import Link from "next/link";

const Footer: React.FC = () => {
  return (
    <footer className="flex justify-center items-center gap-x-10 p-10 ">
      <p>An RCOS Project</p>
      <Link href="https://github.com/SearchRPI/Search/tree/main">
        {/* FIX: Company icons are removed from Lucide-React, need to find a replacement as it is deprecated */}
        <GithubIcon />
      </Link>
    </footer>
  );
};

export default Footer;
