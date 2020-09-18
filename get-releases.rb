#!/usr/bin/env ruby
# note that when this script downloads a file, it also increments the download counter for those files.

require "date"
require "json"
require "fileutils"
require "http"

def github_get_all_pages(url)
  entries = []
  page = 1
  while true
    resp = HTTP.get("#{url}?page=#{page}")
    raise(resp) if resp.code != 200
    data = JSON.parse(resp.to_s)
    break if data.length == 0
    entries = entries + data
    page += 1
  end
  entries
end

def humanize(number)
  number.to_s.reverse.gsub(/(\d{3})(?=\d)/, "\\1,").reverse
end

filetype_downloads = {}
total_downloads = 0
no_binaries = ARGV.include?("--no-binaries")

repo = %x[git remote -v][/(?<=:)[^ ]+(?=\.git)/]
puts "Repo: #{repo}"

puts "Getting releases..."
releases = github_get_all_pages("https://api.github.com/repos/#{repo}/releases")

File.open("releases.json", "wb") { |f| f.write(releases.to_json) }

puts "Found #{releases.length} releases!"

File.open("README.md", "wb") do |readme|
  tbl1_col2_width = releases.map { |rel| rel["tag_name"].length }.max * 2 + 4
  tbl1_col3_width = [humanize(releases.map { |rel| rel["assets"].map { |asset| asset["download_count"] }.sum }.max).length, 9].max

  readme.write <<~eos
    # #{repo}

    ## Downloads by release

    Date       | Release#{" "*(tbl1_col2_width-7)} | #{" "*(tbl1_col3_width-9)}Downloads
    ---------- | #{"-"*tbl1_col2_width} | #{"-"*tbl1_col3_width}
  eos

  releases.sort_by { |rel| rel["created_at"] }.reverse!.each do |rel|
    release_downloads = 0
    puts
    puts rel["tag_name"]
    FileUtils.mkdir_p(rel["tag_name"])

    File.open("#{rel["tag_name"]}/README.md", "wb") do |tag_readme|
      tag_readme.write("# #{rel["name"]}\n\n")

      body = rel["body"].chomp
      if body != ""
        tag_readme.write("#{body}\n")
      end

      tbl2_col1_width = [rel["assets"].map { |asset| asset["name"].length }.max, 8].max
      tbl2_col2_width = [humanize(rel["assets"].map { |asset| asset["download_count"] }.max).length, 14].max

      tag_readme.write <<~eos

        ## Downloads by filename

        Filename#{" "*(tbl2_col1_width-8)} | #{" "*(tbl2_col2_width-14)}Download Count
        #{"-"*tbl2_col1_width} | #{"-"*tbl2_col2_width}
      eos

      rel["assets"].sort_by { |asset| asset["name"] }.each do |asset|
        tag_readme.printf("%-#{tbl2_col1_width}s | %#{tbl2_col2_width}s\n", asset["name"], humanize(asset["download_count"]))
        total_downloads += asset["download_count"]
        release_downloads += asset["download_count"]

        ext = File.extname(asset["name"])
        filetype_downloads[ext] ||= 0
        filetype_downloads[ext] += asset["download_count"]

        puts "- #{asset["name"]} (#{humanize(asset["download_count"])})"
        path = "#{rel["tag_name"]}/#{asset["name"]}"

        next if no_binaries
        next if File.exists?(path) && File.size(path) == asset["size"]

        resp = HTTP.follow.get(asset["browser_download_url"])
        raise(resp) if resp.code != 200
        open(path, "wb") do |io|
          resp.body.each do |chunk|
            io.write(chunk)
          end
        end
      end
      date = Date.parse(rel["created_at"])
      readme.printf("%s | %-#{tbl1_col2_width}s | %#{tbl1_col3_width}s\n", date.strftime("%Y-%m-%d"), "[#{rel["tag_name"]}](#{rel["tag_name"]})", humanize(release_downloads))
    end
  end

  tbl3_col1_width = [filetype_downloads.keys.map { |ext| ext.length }.max, 8].max
  tbl3_col2_width = [humanize(filetype_downloads.values.max).length, 14].max

  readme.write <<~eos

    ## Downloads by filetype

    Filetype#{" "*(tbl3_col1_width-8)} | #{" "*(tbl3_col2_width-14)}Download Count
    #{"-"*tbl3_col1_width} | #{"-"*tbl3_col2_width}
  eos
  filetype_downloads.sort_by { |_,v| v }.reverse.each do |filetype, downloads|
    readme.printf("%-#{tbl3_col1_width}s | %#{tbl3_col2_width}s\n", filetype, humanize(downloads))
  end
  readme.write("\nTotal downloads: #{humanize(total_downloads)}\n")
end
