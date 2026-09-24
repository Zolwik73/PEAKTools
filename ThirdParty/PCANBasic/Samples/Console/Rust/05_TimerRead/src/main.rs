mod PCANBasic;
#[path = "05_TimerRead.rs"]
mod TimerRead;

#[tokio::main]
async fn main() {
    TimerRead::run().await;
}
