import Footer from "@/components/footer";
import Header from "../components/header";

const HomePage: React.FC = () => {
  return (
    <div>
      <Header />
      <div className="h-[90vh]">Hello World</div>
      <Footer />
    </div>
  );
};

export default HomePage;
