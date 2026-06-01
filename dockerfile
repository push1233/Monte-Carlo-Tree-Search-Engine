# 1. Start with a fresh Ubuntu Linux OS
FROM ubuntu:22.04

# 2. Install system dependencies (C++ compilers, Make, Node.js)
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    curl \
    && curl -fsSL https://deb.nodesource.com/setup_20.x | bash - \
    && apt-get install -y nodejs \
    && rm -rf /var/lib/apt/lists/*

# 3. Set the working directory inside the container
WORKDIR /app

# 4. Copy all the source code into the container
COPY . .

# 5. Compile the C++ Engine
WORKDIR /app/build
RUN make

# 6. Build the React Frontend
WORKDIR /app/frontend
RUN npm install
RUN npm run build

# 7. Install backend dependencies
WORKDIR /app
RUN npm install

# 8. Expose the port and boot the server
EXPOSE 3000
CMD ["node", "srv.js"]
