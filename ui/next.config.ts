import type { NextConfig } from "next";

const nextConfig: NextConfig = {
  /* config options here */
};

module.exports = {
  // Overwrites the local server that is being ran for the REST API
  async rewrites() {
    return [
      {
        source: '/api/search',
        destination: 'http://0.0.0.0:8080/search',
      },
    ]
  },
}

export default nextConfig;
