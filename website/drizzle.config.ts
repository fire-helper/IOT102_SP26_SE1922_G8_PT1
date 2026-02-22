import { defineConfig } from "drizzle-kit"

export default defineConfig({
	out: "./drizzle",
	schema: "./src/lib/schemas.ts",
	dialect: "sqlite",
	dbCredentials: {
		url: "data.db"
	}
})
