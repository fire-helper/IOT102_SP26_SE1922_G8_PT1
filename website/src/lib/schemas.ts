import { int, sqliteTable, text } from "drizzle-orm/sqlite-core"

export const logsTable = sqliteTable("logs_table", {
	id: int().primaryKey({ autoIncrement: true }),
	text: text().notNull(),
	timestamp: text().notNull()
})
